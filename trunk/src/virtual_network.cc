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
#include <sstream>
namespace tincan
{
VirtualNetwork::VirtualNetwork(
  unique_ptr<VnetDescriptor> descriptor,
  ControllerHandle & ctrl_handle) :
  tdev_(nullptr),
  peer_network_(nullptr),
  descriptor_(move(descriptor)),
  ctrl_handle_(ctrl_handle)
{
  tdev_ = new TapDev(
    descriptor_->tap_name,
    make_unique<AsyncRead>(
      AsyncRead(make_unique<ReadCompletion>(
        ReadCompletion(fqr_)))),
    make_unique<AsyncWrite>(
      AsyncWrite((make_unique<WriteCompletion>(
        WriteCompletion(fqw_))))));

  peer_network_ = new PeerNetwork(descriptor->tap_name);
}

VirtualNetwork::~VirtualNetwork()
{
  //delete xmpp_network_;
  delete peer_network_;
  delete tdev_;
}

void
VirtualNetwork::Configure()
{
  //initialize the Tap Device
  tdev_->Open();
  //tdev_->SetIp4Addr();
  //tdev_->SetIp4Route();
  
  // we create X509 identity for secure connections
  sslid_.reset(rtc::SSLIdentity::Generate(descriptor_->uid, rtc::KT_RSA));
  local_fingerprint_.reset(rtc::SSLFingerprint::Create(
    rtc::DIGEST_SHA_1, sslid_.get()));

}

void
VirtualNetwork::Start()
{
  tdev_->Up();
  tdev_->StartRead();
  //vlink_thread_.Run();
}

void
VirtualNetwork::Shutdown()
{
  tdev_->Down();
}

void
VirtualNetwork::AddRemotePeer(
  const PeerDescriptor & peer_desc,
  unique_ptr<const VlinkDescriptor> vlink_desc)
{
  void (FrameHandler::*handler)(TapFrame & frame, VirtualLink & vlink) = &FrameHandler::ProcessIncomingFrame;

  if(descriptor_->switchmode_enabled) {
    handler = &FrameHandler::SwitchmodeProcessIncomingFrame;
  }
  unique_ptr<RemotePeer> rp = make_unique<RemotePeer>(peer_desc);
  unique_ptr<VirtualLink> vl = make_unique<VirtualLink>(
    move(vlink_desc), 
    make_unique<IncomingFrameHandler>(*this, handler),
    *this);

  vl->Initialize(descriptor_->uid, 
    net_manager_, 
    *local_fingerprint_.get(), 
    *sslid_.get());

  rp->SetVirtualLink(move(vl));
  peer_network_->Add(move(rp));
}

void VirtualNetwork::RemoveRemotePeer(
  const string & peer_uid)
{
  //RemotePeer & rp = peer_network_->UidToPeer(peer_uid);
  //rp.TrimLink();
  peer_network_->Remove(peer_uid);
}

VnetDescriptor &
VirtualNetwork::LocalDescriptor()
{
  return *descriptor_.get();
}

const string
VirtualNetwork::Name()
{
  return descriptor_->tap_name;
}

const string
VirtualNetwork::HWAddress()
{
  unique_ptr<BYTE[]> hwa = tdev_->GetMacAddress();
  return tdev_->MacAsString(move(hwa));
}

const string
VirtualNetwork::Fingerprint()
{
  return local_fingerprint_->ToString();
}

void
VirtualNetwork::IgnoredNetworkInterfaces(
  const vector<string>& ignored_list)
{
  net_manager_.set_network_ignore_list(ignored_list);
}

void 
VirtualNetwork::ProcessIncomingFrame(
  TapFrame & frame, VirtualLink & vlink)
{
  if(frame.IsIccMsg()) {
    ctrl_handle_.Deliver(frame);
  }
  else if(1) {
    //process switch mode ARP request
    vlink.Transmit(frame);
  }

  tdev_->Write(frame);
}

/*
Performs tunneling and injects the packet into the the local
system.
*/
void 
VirtualNetwork::ProcessOutgoingFrame(
  TapFrame & frame, VirtualLink & vlink)
{
  //TODO: tunneling
  vlink.Transmit(frame);
}

void VirtualNetwork::SwitchmodeProcessIncomingFrame(TapFrame & frame, VirtualLink & vlink)
{}

void VirtualNetwork::SwitchmodeProcessOutgoingFrame(TapFrame & frame, VirtualLink & vlink)
{}

} //namespace tincan
