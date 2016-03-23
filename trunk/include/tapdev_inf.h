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
#ifndef TINCAN_TAPDEV_INF_H_
#define TINCAN_TAPDEV_INF_H_

#include "tap_frame.h"

namespace tincan
{

class TapDevInf
{
public:
  virtual void Open(
    const string & device_name) = 0;

  virtual void Close() = 0;

  virtual void StartRead() = 0;

  virtual void Write(
    TapFrame & frame) = 0;

  virtual void EnableArp() = 0;

  virtual void Up() = 0;

  virtual void Down() = 0;

  virtual void SetMtu(
    int mtu) = 0;
  /**
  * Sets the IPv4 address for the device, given a string, such as
  * "172.31.0.100". The `prefix_len` specifies how many bits are specifying the
  * subnet, aka the routing prefix or the mask length.
  */
  virtual void SetIp4Addr(
    const string & presentation,
    unsigned int prefix_len) = 0;

  virtual void GetIp4Address(
    unique_ptr<BYTE[]> ip4,
    unsigned int ip4_len) = 0;

  /**
  * Sets the IPv6 address for the device, given a string, such as
  * "fd50:0dbc:41f2:4a3c:0:0:0:1000". The `prefix_len` specifies how many bits
  * are specifying the subnet, aka the routing prefix (refered to as the mask in
  * IPv4).
  */
  virtual void SetIp6Addr(
    const string & presentation,
    unsigned int prefix_len) = 0;
  
  virtual void GetIp6Address(
    unique_ptr<BYTE[]> ip6,
    unsigned int ip6_len) = 0;

  /**
  * Tells the OS to route IPv4 addresses within the subnet (determined by the
  * `presentation` and `prefix_len` args, see tap_set_ipv4_addr) through us. A
  * priority is given by metric. The Linux kernel's default metric value is 256
  * for subnets and 1024 for gateways.
  */
  virtual void SetIp4Route(
    const string & presentation,
    unsigned short prefix_len,
    unsigned int metric) = 0;

  virtual void SetIp6Route(
    const string & presentation,
    unsigned short prefix_len,
    unsigned int metric) = 0;

  virtual void DisableIp6Autoconfig() = 0;

  virtual void SetIp4ProcOption(
    const string & option,
    const string & value) = 0;

  virtual void SetIp6ProcOption(
    const string & option,
    const string & value) = 0;
};

}  // namespace tincan
#endif  // TINCAN_TAPDEV_H_