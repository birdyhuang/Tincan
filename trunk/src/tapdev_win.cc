/*
* ipop-project
* Copyright 2016, University of Florida
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#if defined(WINDOWS)

#include "tapdev_win.h"
#include <iphlpapi.h>
#include <stdio.h>
#include <wchar.h>
#include <winsock2.h>
#include "win_exception.h"
#define _WINSOCK2API_


namespace tincan 
{
namespace win
{
VOID CALLBACK ReadCompletionRoutine(
  DWORD dwErrorCode,
  DWORD dwNumberOfBytesTransfered,
  LPOVERLAPPED lpOverlapped
  )
{
  AsyncIo<ReadCompletion> * aio = (AsyncIo<ReadCompletion> *)lpOverlapped;
  (*aio->completion)(aio->frame);
}

const char * const TapDevWin::NETWORK_PATH_ = "SYSTEM\\CurrentControlSet\\Control\\Network";
const char * const TapDevWin::USER_MODE_DEVICE_DIR_ = "\\\\.\\Global\\";
const char * const TapDevWin::TAP_SUFFIX_ = ".tap";

#define TAP_CONTROL_CODE(request,method) \
  CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)

TapDevWin::TapDevWin():
  is_read_started_(false)
{}


TapDevWin::~TapDevWin()
{}

void 
TapDevWin::Open(
  const string & device_name)
{
  int len = 0, status = 1;
  string device_guid;

  NetDeviceNameToGuid(device_name, device_guid);
  unique_ptr<BYTE[]>mac_adress = GetMacAddress(device_name);
  string device_path(USER_MODE_DEVICE_DIR_);
  device_path.append(device_guid).append(TAP_SUFFIX_);
  auto dev_handle = CreateFile(device_path.c_str(),
    GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 
    FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED, 0);
  if(INVALID_HANDLE_VALUE != dev_handle) {
    iocmpl_.SetDevHandle(dev_handle);
    if(!DeviceIoControl(iocmpl_.dev_handle, TAP_IOCTL_SET_MEDIA_STATUS,
      &status, sizeof(status), &status, sizeof(status), (LPDWORD)&len, NULL))
      throw WINEXCEPT("Failed DeviceIoControl");
  }
  else {
    throw WINEXCEPT("Failed CreateFile");
  }
  iocmpl_.read_overlap.io_cmpl_routine = ReadCompletionRoutine;
 ? iocmpl_.read_overlap.completion = new ReadCompletion(*new FrameQueue);
}

void
TapDevWin::StartRead()
{
  if(is_read_started_)
    return;//assert?
  is_read_started_ = true;
  ReadFileEx(iocmpl_.dev_handle,
    iocmpl_.read_overlap.frame.buffer.get(),
    iocmpl_.read_overlap.frame.frame_sz,
    (LPOVERLAPPED)&iocmpl_.read_overlap,
    (LPOVERLAPPED_COMPLETION_ROUTINE)iocmpl_.read_overlap.io_cmpl_routine);
  return;
}

void
TapDevWin::NetDeviceNameToGuid(
  const string & name,
  string & guid)
{
  int i_0 = 0, i_1 = 0;
  bool found = false;
  DWORD size = 256;
  HKEY key_0, key_1, key_2;
  char name_0[256], name_1[256], name_2[256];
  string full_path;
  try {
    /* Open up Networking in the registry */
    if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NETWORK_PATH_, 0,
      KEY_READ, &key_0)) {
      string emsg("Failed to open registry key");
      emsg.append(NETWORK_PATH_);
      throw WINEXCEPT(emsg.c_str());
    }

    for(i_0 = 0; ; i_0++) {
      /* Enumerate through the different keys under Network\layer0 */
      if(RegEnumKeyEx(key_0, i_0, name_0, &size, NULL, NULL, NULL, NULL)
        != ERROR_SUCCESS) {
        continue;
      }
      full_path.assign(NETWORK_PATH_).append("\\").append(name_0);
      /* Open the current key we're enumerating through Network\layer0 */
      RegCloseKey(key_1);
      if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, full_path.c_str(), 0,
        KEY_READ, &key_1)) {
        string emsg("Failed to open registry key");
        emsg.append(full_path);
        throw WINEXCEPT(emsg.c_str());
      }
      for(i_1 = 0; ; i_1++) {
        size = 255;
        /* This enumerates through the next layer Network\layer0\layer1 */
        if(RegEnumKeyEx(key_1, i_1, name_1, &size, NULL, NULL, NULL, NULL)
          != ERROR_SUCCESS) {
          i_1 = -1;
          break;
        }

        full_path.append(name_1);
        /* This opens keys that we're looking for, if they don't exist, let's continue */
        RegCloseKey(key_2);
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, full_path.c_str(), 0,
          KEY_READ, &key_2) != ERROR_SUCCESS) {
          continue;
        }
        size = 255;
        /* We get the Name of the network interface, if it matches, let's get the GUID and return */
        if(ERROR_SUCCESS != RegQueryValueEx(key_2, "Name", 0, NULL, (LPBYTE)
          name_2, &size)) {
          continue;
        }
        if(name.compare(name_2) == 0) {
          guid.assign(name_1);
          found = true;
        }
        RegCloseKey(key_2);
      }
      RegCloseKey(key_1);
    }
    RegCloseKey(key_0);
  }
  catch(exception & e) {
    RegCloseKey(key_2);
    RegCloseKey(key_1);
    RegCloseKey(key_0);
  }
  return;
}

unique_ptr<BYTE[]>
TapDevWin::GetMacAddress(
  const string & device_name)
{
  unique_ptr<BYTE[]> mac_address;
  int num_addresses = 16;
  ULONG adptbuflen = num_addresses * sizeof(IP_ADAPTER_ADDRESSES);
  wchar_t tmp_name[100];
  wchar_t w_device_name[100];
  int result = 0;

  unique_ptr<IP_ADAPTER_ADDRESSES[]> pAddresses(
    new IP_ADAPTER_ADDRESSES[num_addresses]);
  if(ERROR_SUCCESS != GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX,
    NULL, pAddresses.get(), &adptbuflen)) {
    throw WINEXCEPT("GetAdaptersAddresses failed");
  }
  num_addresses = adptbuflen/sizeof(IP_ADAPTER_ADDRESSES);
  for(int i = 0; i < num_addresses; i++) {
    swprintf(tmp_name, 100, L"%s", pAddresses[i].FriendlyName);
    swprintf(w_device_name, 100, L"%hs", device_name.c_str());
    if(wcscmp(tmp_name, w_device_name) == 0) {
      mac_address.reset(new BYTE[MAX_ADAPTER_ADDRESS_LENGTH]);
      memset(mac_address.get(), 0, MAX_ADAPTER_ADDRESS_LENGTH);
      memcpy(mac_address.get(), pAddresses[i].PhysicalAddress,
        pAddresses[i].PhysicalAddressLength);
      break;
    }
  }
  return move(mac_address);
}

}  // namespace win
}  // namespace tincan
#endif// WINDOWS
