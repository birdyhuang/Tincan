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
#if defined(_IPOP_WIN)

#include "windows/tapdev_win.h"
#include <iphlpapi.h>
#include <stdio.h>
#include <wchar.h>
#include <winioctl.h>
#include "windows/win_exception.h"


namespace tincan 
{
namespace windows
{
const char * const TapDevWin::NETWORK_PATH_ = "SYSTEM\\CurrentControlSet\\Control\\Network";
const char * const TapDevWin::USER_MODE_DEVICE_DIR_ = "\\\\.\\Global\\";
const char * const TapDevWin::TAP_SUFFIX_ = ".tap";

#define TAP_CONTROL_CODE(request,method) \
  CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)

VOID CALLBACK
ReadCompletionRoutine(
  DWORD dwErrorCode,
  DWORD dwNumberOfBytesTransfered,
  LPOVERLAPPED lpOverlapped
  )
{
  //check read successful
  AsyncRead * rd_overlap = (AsyncRead*)lpOverlapped;
  ReadCompletion & rdcompl = *rd_overlap->completion.get();
  rdcompl(rd_overlap->frame);  //packet processing
  //issue new async read request
  ReadFileEx(rd_overlap->dev_handle,
    rd_overlap->frame.Data(),
    rd_overlap->frame.Capacity(),
    (LPOVERLAPPED)&rd_overlap,
    (LPOVERLAPPED_COMPLETION_ROUTINE)ReadCompletionRoutine);

}

VOID CALLBACK
WriteCompletionRoutine(
  DWORD dwErrorCode,
  DWORD dwNumberOfBytesTransfered,
  LPOVERLAPPED lpOverlapped
  )
{
  //check write successful
  AsyncWrite * wr_overlap = (AsyncWrite*)lpOverlapped;
  WriteCompletion & wrcompl = *wr_overlap->completion.get();
  wrcompl(wr_overlap->frame);//get data for new write
  //issue new async write request
  WriteFileEx(wr_overlap->dev_handle,
    wr_overlap->frame.Data(),
    wr_overlap->frame.Capacity(),
    (LPOVERLAPPED)&wr_overlap,
    (LPOVERLAPPED_COMPLETION_ROUTINE)WriteCompletionRoutine);
}

TapDevWin::TapDevWin():
  is_read_started_(false)
{}

TapDevWin::TapDevWin(
  const string & tap_name,
  unique_ptr<AsyncRead>async_rd,
  unique_ptr<AsyncWrite> async_wr) :
  tap_name_(tap_name),
  rd_overlap_(move(async_rd)),
  wr_overlap_(move(async_wr)),
  is_read_started_(false)
{}

TapDevWin::~TapDevWin()
{}

void
TapDevWin::Open()
{
  int len = 0, status = 1;
  string device_guid;

  NetDeviceNameToGuid(tap_name_, device_guid);
  unique_ptr<BYTE[]>mac_adress = GetMacAddress();
  string device_path(USER_MODE_DEVICE_DIR_);
  device_path.append(device_guid).append(TAP_SUFFIX_);
  auto dev_handle = CreateFile(device_path.c_str(),
    GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
    FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED, 0);
  if(INVALID_HANDLE_VALUE != dev_handle) {
    SetDevHandle(dev_handle);
    if(!DeviceIoControl(dev_handle, TAP_IOCTL_SET_MEDIA_STATUS,
      &status, sizeof(status), &status, sizeof(status), (LPDWORD)&len, NULL))
      throw WINEXCEPT("Failed DeviceIoControl");
  }
  else {
    throw WINEXCEPT("Failed CreateFile");
  }
}

void 
TapDevWin::Open(
  const string & tap_name,
  unique_ptr<AsyncRead>async_rd,
  unique_ptr<AsyncWrite> async_wr)
{
  tap_name_ = tap_name;
  rd_overlap_ = move(async_rd);
  wr_overlap_ = move(async_wr);

  TapDevWin::Open();
}

void
TapDevWin::StartRead()
{
  if(is_read_started_)
    return;//assert?
  //if(!rd_overlap_->frame.FrameData())
  //  rd_overlap_->frame.buffer = make_unique<BYTE[]>(rd_overlap_->frame.sz);
  is_read_started_ = true;
  ReadFileEx(rd_overlap_->dev_handle,
    rd_overlap_->frame.Data(),
    (unsigned long)rd_overlap_->frame.Capacity(),
    (LPOVERLAPPED)&rd_overlap_,
    (LPOVERLAPPED_COMPLETION_ROUTINE)ReadCompletionRoutine);
  return;
}

void
TapDevWin::Write(
  TapFrame & aframe)
{
  //wr_overlap_->frame = aframe;
  WriteFileEx(wr_overlap_->dev_handle,
    wr_overlap_->frame.Data(),
    (unsigned long)wr_overlap_->frame.Capacity(),
    (LPOVERLAPPED)&wr_overlap_,
    (LPOVERLAPPED_COMPLETION_ROUTINE)WriteCompletionRoutine);
  return;
}

void
TapDevWin::NetDeviceNameToGuid(
  const string & name,
  string & guid)
{
  DWORD i_0 = 0, i_1 = 0;
  bool found = false;
  DWORD size = 256;
  HKEY key_0 = 0, key_1 = 0, key_2 = 0;
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

void TapDevWin::SetDevHandle(HANDLE handle)
{
  rd_overlap_->dev_handle = wr_overlap_->dev_handle = handle;
}

unique_ptr<BYTE[]>
TapDevWin::GetMacAddress() const
{
  unique_ptr<BYTE[]> mac_address;
  size_t num_addresses = 16;
  ULONG adptbuflen = (ULONG)num_addresses * sizeof(IP_ADAPTER_ADDRESSES);
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
    swprintf(w_device_name, 100, L"%hs", tap_name_.c_str());
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

const string
TapDevWin::MacAsString(
  unique_ptr<BYTE[]> hw_address) const
{
  ostringstream mac;
  for(short i = 0; i < 6; ++i) {
    if(i != 0) mac << ':';
    mac.width(2);
    mac.fill('0');
    mac << std::hex << ((int)hw_address[i] & 0xff);
  }
  return mac.str();
}

void
TapDevWin::EnableArp() {}

void
TapDevWin::Up() {}

void
TapDevWin::Down() {}

void
TapDevWin::SetMtu(
  int mtu)
{}

void
TapDevWin::SetIp4Addr(
  const string & presentation,
  unsigned int prefix_len)
{}

void
TapDevWin::GetIp4Address(
  unique_ptr<BYTE[]> ip4,
  unsigned int ip4_len) const
{}

void
TapDevWin::SetIp6Addr(
  const string & presentation,
  unsigned int prefix_len)
{}

void
TapDevWin::GetIp6Address(
  unique_ptr<BYTE[]> ip6,
  unsigned int ip6_len) const
{}

void
TapDevWin::SetIp4Route(
  const string & presentation,
  unsigned short prefix_len,
  unsigned int metric)
{}

void
TapDevWin::SetIp6Route(
  const string & presentation,
  unsigned short prefix_len,
  unsigned int metric)
{}

void
TapDevWin::DisableIp6Autoconfig() {}

void
TapDevWin::SetIp4ProcOption(
  const string & option,
  const string & value)
{}

void
TapDevWin::SetIp6ProcOption(
  const string & option,
  const string & value)
{}
}  // namespace win
}  // namespace tincan
#endif// WINDOWS
