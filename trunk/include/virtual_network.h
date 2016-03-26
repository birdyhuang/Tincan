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
#ifndef TINCAN_VIRTUAL_NIC_H_
#define TINCAN_VIRTUAL_NIC_H_

#include "tapdev.h"
#include <memory>
#include "peer_network.h"
#include "vnet_endpoint_config.h"
#include "xmppnetwork.h"
#include "webrtc/base/thread.h"

namespace tincan {

class VirtualNetwork {
 public:
   VirtualNetwork(unique_ptr<LocalVnetEndpointConfig> lvncfg);
  ~VirtualNetwork();
  void Configure();
  void Start();
  void Shutdown();
private:
  XmppNetwork xmpp_network;
  TapDev * tdev_;
  PeerNetwork peer_network_;
  unique_ptr<LocalVnetEndpointConfig> config_;

  rtc::Thread vlink_thread_;
  FrameQueue fqr_, fqw_;
};
}  // namespace tincan
#endif  // TINCAN_VIRTUAL_NIC_H_
