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
#ifndef TINCAN_TAPDEV_WIN_H_
#define TINCAN_TAPDEV_WIN_H_

#if defined(WINDOWS)

#include <string>
using namespace std;
#include "tapdev_inf.h"
#include "async_io.h"

namespace tincan 
{
namespace win 
{

class TapDevWin : public TapDevInf {
public:
  TapDevWin(
    AsyncIoCompletion & iocmpl);

  ~TapDevWin();

  int Open(
    const string & devname,
    const string & mac);

  void Close();

  int Read(
    TapFrame & frame);

  int Write(
    TapFrame & frame);

  int Configure(
    unsigned long request,
    void* arg);

  int EnableArp();

  int Up();

  int Down();

  int SetMtu(
    int mtu);

  int SetIp4Addr(
    const char *presentation,
    unsigned int prefix_len,
    char *my_ip4);

  int SetIp6Addr(
    const char *presentation,
    unsigned int prefix_len);

  int SetIp4Route(
    const char *presentation,
    unsigned short prefix_len,
    unsigned int metric);

  int SetIp6Route(
    const char *presentation,
    unsigned short prefix_len,
    unsigned int metric);

  int DisableIp6Autoconfig();

  int SetIp4ProcOption(const char *option,
    const char *value);

  int SetIp6ProcOption(
    const char *option,
    const char *value);

protected:
  int SetFlags();
  void NetDeviceNameToGuid(
    const string & name,
    string & guid);
  static const char * const NETWORK_PATH;
  static const char * const USER_MODE_DEVICE_DIR;
  static const char * const TAP_SUFFIX;
};
}  // namespace win
}  // namespace tincan
#endif  // WINDOWS
#endif  // TINCAN_TAPDEV_WIN_H_
