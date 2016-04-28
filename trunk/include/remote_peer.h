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
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdint.h>
#endif // defined(_IPOP_WIN)
#include "controller_handle.h"
#include "virtual_link.h"
#include "vnet_descriptor.h"
namespace tincan
{

class RemotePeer
{
public:
  RemotePeer(
    PeerDescriptor const & descriptor);
	~RemotePeer();

  void SetVirtualLink(
    unique_ptr<VirtualLink> vlink);

  void TrimLink();
  const string & VirtIp4();
  const string & VirtIp6();
  const string & Uid();
  const string & MacAddress();

private:
  const string & uid_;   // 160bit unique identifier
  const string & vip4_;  // the virtual IPv4 address that we see
  const string & vip6_;  // the virtual IPv6 address that we see
  // the actual address we send data to is in the CAS
  unique_ptr<VirtualLink> vlink_;
  size_t last_time;
  bool is_connected; // hmm???
};
} // namespace tincan
