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


#include <memory>
#pragma warning( push )
#pragma warning(disable:4996)
#pragma warning(disable:4100)
#include "webrtc/base/network.h"
#include "webrtc/base/sslidentity.h"
#include "webrtc/base/thread.h"
#pragma warning( pop )
#include "controller_handle.h"
#include "peer_network.h"
#include "tapdev.h"
#include "tap_frame.h"
#include "vnet_descriptor.h"

namespace tincan
{

class VirtualNetwork : public FrameHandler
{
 public:
   VirtualNetwork(
     unique_ptr<VnetDescriptor> descriptor,
     ControllerHandle & ctrl_handle);

  ~VirtualNetwork();
  void Configure();
  void Start();
  void Shutdown();
  void AddRemotePeer(
    const PeerDescriptor & peer_desc,
    unique_ptr<const VlinkDescriptor> vlink_desc);

  void RemoveRemotePeer(
    const string & peer_uid);

  VnetDescriptor & LocalDescriptor();

  const string Name();
  const string HWAddress();
  const string Fingerprint();
  const rtc::SSLFingerprint & LocalFingerprint();

  void IgnoredNetworkInterfaces(
    const vector<string>& ignored_list);
  //
  //FrameHandler interface implementation
  void ProcessIncomingFrame(
    TapFrame & frame, VirtualLink & vlink);

  void ProcessOutgoingFrame(
    TapFrame & frame, VirtualLink & vlink);
  
  void SwitchmodeProcessIncomingFrame(
    TapFrame & frame, VirtualLink & vlink);

  void SwitchmodeProcessOutgoingFrame(
    TapFrame & frame, VirtualLink & vlink);

private:
  rtc::BasicNetworkManager net_manager_;
  unique_ptr<rtc::SSLIdentity> sslid_;
  unique_ptr<rtc::SSLFingerprint> local_fingerprint_;
  unique_ptr<VnetDescriptor> descriptor_;
  TapDev * tdev_;
  PeerNetwork * peer_network_;
  ControllerHandle & ctrl_handle_;
  //rtc::Thread vlink_thread_;
  //XmppNetwork * xmpp_network_;
  FrameQueue fqr_, fqw_;
};
}  // namespace tincan
#endif  // TINCAN_VIRTUAL_NIC_H_
