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

#if defined(_IPOP_WIN)

#include <string>
using namespace std;
#include <memory>
#include <Winsock2.h>
#include "async_io.h"
#include "tapdev_inf.h"

namespace tincan
{
namespace windows
{
class TapDevWin : public TapDevInf
{
public:
  TapDevWin();
  TapDevWin(
    const string & tap_name,
    unique_ptr<AsyncRead>async_rd,
    unique_ptr<AsyncWrite> async_wr);

  ~TapDevWin();

  void Open();
  void Open(
    const string & tap_name,
    unique_ptr<AsyncRead>async_rd,
    unique_ptr<AsyncWrite> async_wr);

  void Close() {}

  void StartRead();

  void Write(
    TapFrame & frame);

  void EnableArp();

  void Up();

  void Down();

  void SetMtu(
    int mtu);

  void SetIp4Addr(
    const string & presentation,
    unsigned int prefix_len);

  void GetIp4Address(
    unique_ptr<BYTE[]> ip4,
    unsigned int ip4_len) const;

  void SetIp6Addr(
    const string & presentation,
    unsigned int prefix_len);

  void GetIp6Address(
    unique_ptr<BYTE[]> ip6,
    unsigned int ip6_len) const;

  void SetIp4Route(
    const string & presentation,
    unsigned short prefix_len,
    unsigned int metric);

  void SetIp6Route(
    const string & presentation,
    unsigned short prefix_len,
    unsigned int metric);

  void DisableIp6Autoconfig();

  void SetIp4ProcOption(
    const string & option,
    const string & value);

  void SetIp6ProcOption(
    const string & option,
    const string & value);

  unique_ptr<BYTE[]> GetMacAddress() const;

  const string MacAsString(
    unique_ptr<BYTE[]>hw_address) const;

protected:
//  int SetFlags();
  void NetDeviceNameToGuid(
    const string & name,
    string & guid);
  
  void SetDevHandle(
    HANDLE handle);
  
  static const char * const NETWORK_PATH_;
  static const char * const USER_MODE_DEVICE_DIR_;
  static const char * const TAP_SUFFIX_;
  bool is_read_started_;
  unique_ptr<AsyncRead> rd_overlap_;
  unique_ptr<AsyncWrite> wr_overlap_;
  string tap_name_;
};
}  // namespace win
}  // namespace tincan
#endif  // _IPOP_WIN
#endif  // TINCAN_TAPDEV_WIN_H_
