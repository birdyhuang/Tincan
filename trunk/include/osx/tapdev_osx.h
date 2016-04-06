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
#ifndef TINCAN_TAPDEV_MAC_H_
#define TINCAN_TAPDEV_MAC_H_

#include "tapdev_inf.h"
#include "async_io.h"

namespace tincan {

namespace osx {
    
class TapDevOsx : public TapDevInf
{
public:
//  TapDevMac(
//    AsyncIoCompletion & iocmpl);
    TapDevOsx();

  ~TapDevOsx();

  void Open(const string &device);

  void Close();

  void Read(
    TapFrame & frame);

  void Write(
    TapFrame & frame);

  void Configure(
    unsigned long request,
    void* arg);

  void EnableArp();

  void Up();

  void Down();

  void SetMtu(
    int mtu);

  int SetIp4Addr(
    const char *presentation,
    unsigned int prefix_len,
    char *my_ip4);

  int SetIp6Addr(
    const char *presentation,
    unsigned int prefix_len);

//  int SetIp4Route(
//    const char *presentation,
//    unsigned short prefix_len,
//    unsigned int metric);
//
//  int SetIp6Route(
//    const char *presentation,
//    unsigned short prefix_len,
//    unsigned int metric);
//
//  int DisableIp6Autoconfig();
//
//  int SetIp4ProcOption(const char *option,
//    const char *value);
//
//  int SetIp6ProcOption(
//    const char *option,
//    const char *value);

    unique_ptr<BYTE[]> getMacAddress(const string & device_name);

protected:
    /* static defined in the class, but initialized outside the class */
    static const char *const  TUN_PATH;
    static const char *const  PATH_2_IFCONFIG;
    static const char *const  IFCONFIG;
    static int fd;
    static int ipv4_config_sock, ipv6_config_sock;
    static const int MAX_ADAPTER_ADDRESS_LENGTH;
    
  int SetFlags();
};
}  // namespace osx
}  // namespace tincan
#endif  // TINCAN_TAPDEV_MAC_H_
