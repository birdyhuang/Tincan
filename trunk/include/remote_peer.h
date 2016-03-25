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
#endif // defined(windows)
#include "virtual_link.h"

namespace tincan
{
class RemotePeer
{
public:
  RemotePeer();
	~RemotePeer();

private:
  char id[ID_SIZE]; // 160bit unique identifier
  struct in_addr virt_ip; // the virtual IPv4 address that we see
  struct in6_addr virt_ip6; // the virtual IPv6 address that we see
  struct in_addr dest_ip4;  // the actual address to send data to
  unsigned char mac[6]; // MAC address
  uint16_t port; // The open port on the client that we're connected to

  size_t overlay_id;
  size_t last_time;
  std::string uid;
  std::string fingerprint;
  std::string connection_security;

  VirtualLink vlink;
};
} // namespace tincan
