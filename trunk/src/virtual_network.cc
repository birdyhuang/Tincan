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
#include "virtual_network.h"
namespace tincan
{
VirtualNetwork::VirtualNetwork(unique_ptr<LocalVnetEndpointConfig> lvecfg) :
  tdev_(nullptr),
  xmpp_network_(nullptr),
  peer_network_(nullptr),
  config_(move(lvecfg))
{

  tdev_ = new TapDev(
    make_unique<AsyncRead>(
      AsyncRead(make_unique<ReadCompletion>(
        ReadCompletion(fqr_)))),
    make_unique<AsyncWrite>(
      AsyncWrite((make_unique<WriteCompletion>(
        WriteCompletion(fqw_))))));
  peer_network_ = new PeerNetwork;
  xmpp_network_ = new XmppNetwork;
}

VirtualNetwork::~VirtualNetwork()
{
  delete xmpp_network_;
  delete peer_network_;
  delete tdev_;
}
void VirtualNetwork::Configure()
{
  //initialize the Tap Device
  tdev_->Open(config_->tap_name);
  //tdev_->SetIp4Addr();
  //tdev_->SetIp4Route();

  //free the config as its not needed anymore
  config_.reset();
}
void VirtualNetwork::Start()
{
  tdev_->Up();
  tdev_->StartRead();
  vlink_thread_.Run();
}
void VirtualNetwork::Shutdown()
{
  tdev_->Down();
}

void VirtualNetwork::AddRemotePeer(unique_ptr<VnetEndpointConfig> vecfg)
{
  unique_ptr<RemotePeer> rp = make_unique<RemotePeer>(*vecfg.get());
  unique_ptr<VirtualLink> vl = make_unique<VirtualLink>(*this);
  rp->SetVirtialLink(move(vl));
  peer_network_->Add(move(rp));
}

} //namespace tincan
