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
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <wchar.h>
#include "win_exception.h"

namespace tincan 
{
namespace win
{

const char * const TapDevWin::NETWORK_PATH = "SYSTEM\\CurrentControlSet\\Control\\Network";
const char * const TapDevWin::USER_MODE_DEVICE_DIR = "\\\\.\\Global\\";
const char * const TapDevWin::TAP_SUFFIX = ".tap";

TapDevWin::TapDevWin(
  AsyncIoCompletion & iocmpl)
{}


TapDevWin::~TapDevWin()
{}

int Open(
  const string & devname,
  const string & mac)
{
  int len, status = 1;
  char device_path[255], device_guid[255];
  if(network_device_name_to_guid(device_name, device_guid) != 1 ||
    get_mac(device_name, mac) != 1) {
    return (windows_tap *)-1;
  }
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
    if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NETWORK_PATH, 0,
      KEY_READ, &key_0)) {
      string emsg("Failed to open registry key");
      emsg.append(NETWORK_PATH);
      throw WINEXCEPT(emsg.c_str());
    }

    for(i_0 = 0; ; i_0++) {
      /* Enumerate through the different keys under Network\layer0 */
      if(RegEnumKeyEx(key_0, i_0, name_0, &size, NULL, NULL, NULL, NULL)
        != ERROR_SUCCESS) {
        continue;
      }
      full_path.assign(NETWORK_PATH).append("\\").append(name_0);
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
}  // namespace win
}  // namespace tincan
#endif// WINDOWS
