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
#include "virtual_link.h"
#include<memory>
#pragma warning( push )
#pragma warning(disable:4996)
#pragma warning(disable:4100)
#include "webrtc/base/stringencode.h"
#include "webrtc/p2p/base/dtlstransport.h"
#pragma warning( pop )
#include "tap_frame.h"
namespace tincan
{
using namespace std;
using namespace rtc;

VirtualLink::VirtualLink(
  unique_ptr<const VlinkDescriptor> vlink_desc,
  unique_ptr<IncomingFrameHandler> ifc,
  FrameHandler & frame_handler) :
  vlink_desc_(move(vlink_desc)),
  ifc_(move(ifc)),
  frame_handler_(frame_handler),
  tiebreaker_(rtc::CreateRandomId64()),
  content_name_(vlink_desc_->name)
{}

VirtualLink::~VirtualLink()
{}

void
VirtualLink::Initialize(
  const string & local_uid,
  BasicNetworkManager & network_manager,
  const SSLFingerprint & local_fingerprint,
  const SSLIdentity & sslid)
{
  CreateTransport(network_manager, local_fingerprint, sslid);
  RegisterLinkEventHandlers();
  SetupTransport(local_uid, local_fingerprint, vlink_desc_->peer_fpr);
  transport_->ConnectChannels();
  //TODO: Invoke packet handling thread???
}

/* Parses the string delimited list of candidates and adds
them to the P2P transport thereby creating ICE connections
*/
void
VirtualLink::CreateCandidateConnections(
  const string & candidates)
{
  std::istringstream iss(candidates);
  do {
    std::string candidate_str;
    iss >> candidate_str;
    std::vector<std::string> fields;
    size_t len = rtc::split(candidate_str, ':', &fields);
    if(len >= 12) {
      //TODO: fix candidates
      cricket::Candidate candidate;
      //fields[0], 
      //atoi(fields[1].c_str()), 
      //fields[2],
      //rtc::SocketAddress(fields[3], atoi(fields[4].c_str())),
      //atoi(fields[5].c_str()), 
      //fields[6], 
      //fields[7], 
      //fields[8],
      //fields[9], 
      //atoi(fields[10].c_str()), 
      //fields[11]);
      candidates_->push_back(candidate);
    }
  } while(iss);
  string err;
  bool rv = transport_->AddRemoteCandidates(*candidates_.get(), &err);
  return;
}

void
VirtualLink::CreateTransport(
  BasicNetworkManager & network_manager,
  const SSLFingerprint & local_fingerprint,
  const SSLIdentity & sslid)
{
  rtc::SocketAddress stun_addr;
  stun_addr.FromString(vlink_desc_->stun_addr);
  port_allocator_.reset(new cricket::BasicPortAllocator(
    &network_manager, &packet_factory_, { stun_addr }));
  port_allocator_->set_flags(params_.kFlags);

  SetupTURN(vlink_desc_->turn_addr, vlink_desc_->turn_user, vlink_desc_->turn_pass);

  int component = cricket::ICE_CANDIDATE_COMPONENT_DEFAULT;
  //TODO:
  rtc::scoped_refptr<rtc::RTCCertificate> cert;
  //scoped_ptr<SSLIdentity> identity;
  cert->Create(scoped_ptr<SSLIdentity>());
  if(vlink_desc_->sec_enabled) {
    transport_ = make_unique<DtlsP2PTransport>(
      content_name_, port_allocator_.get(), cert);

    cricket::DtlsTransportChannelWrapper* dtls_channel =
      static_cast<cricket::DtlsTransportChannelWrapper*>(
        transport_->CreateChannel(component));

    channel_.reset(static_cast<cricket::P2PTransportChannel*>(
      dtls_channel->channel()));
    connection_security_ = "dtls";
  }
  else {
    transport_.reset(new cricket::P2PTransport(
      content_name_, port_allocator_.get()));
    channel_.reset(static_cast<cricket::P2PTransportChannel*>(
      transport_->CreateChannel(component)));
    connection_security_ = "none";
  }
}

void
VirtualLink::OnReadPacket(
  cricket::TransportChannel * channel,
  const char * data,
  size_t len,
  const rtc::PacketTime & ptime,
  int flags)
{
  //TODO:
  TapFrame frame_((unsigned char*)data, len);
  (*ifc_.get())(frame_, *this);
}

void
VirtualLink::RegisterLinkEventHandlers()
{
  channel_->SignalReadPacket.connect(
    this, &VirtualLink::OnReadPacket);

  //transport_->SignalRequestSignaling.connect(
  //  transport_, &DtlsP2PTransport::OnSignalingReady);

  //transport_->SignalCandidatesReady.connect(
  //  this, &TinCanConnectionManager::OnCandidatesReady);

  //transport_->SignalCandidatesAllocationDone.connect(
  //  this, &TinCanConnectionManager::OnCandidatesAllocationDone);

  //transport_->SignalReadableState.connect(
  //  this, &TinCanConnectionManager::OnRWChangeState);

  //transport_->SignalWritableState.connect(
  //  this, &TinCanConnectionManager::OnRWChangeState);
}

void VirtualLink::Transmit(TapFrame & frame)
{}

void
VirtualLink::SetupTransport(
  const string & local_uid,
  const SSLFingerprint & local_fingerprint,
  const string & peer_fpr)
{
  transport_->SetIceTiebreaker(tiebreaker_);
  remote_fingerprint_.reset(
    rtc::SSLFingerprint::CreateFromRfc4572(rtc::DIGEST_SHA_1, peer_fpr));
  cricket::ConnectionRole conn_role_local = cricket::CONNECTIONROLE_ACTPASS;
  if(vlink_desc_->peer_uid.compare(local_uid) > 0) {
    conn_role_local = cricket::CONNECTIONROLE_ACTIVE;
  }
  local_description_.reset(new cricket::TransportDescription(
    std::vector<std::string>(),
    params_.kIceUfrag,
    params_.kIcePwd,
    cricket::ICEMODE_FULL,
    conn_role_local,
    &local_fingerprint));

  remote_description_.reset(new cricket::TransportDescription(
    std::vector<std::string>(),
    params_.kIceUfrag,
    params_.kIcePwd,
    cricket::ICEMODE_FULL,
    cricket::CONNECTIONROLE_NONE,
    remote_fingerprint_.get()));

  if(vlink_desc_->peer_uid.compare(local_uid) < 0) {
    transport_->SetIceRole(cricket::ICEROLE_CONTROLLING);
    transport_->SetLocalTransportDescription(
      *local_description_.get(), cricket::CA_OFFER, NULL);
    transport_->SetRemoteTransportDescription(
      *remote_description_.get(), cricket::CA_ANSWER, NULL);
  }
  else {
    transport_->SetIceRole(cricket::ICEROLE_CONTROLLED);
    transport_->SetRemoteTransportDescription(
      *remote_description_.get(), cricket::CA_OFFER, NULL);
    transport_->SetLocalTransportDescription(
      *local_description_.get(), cricket::CA_ANSWER, NULL);
  }
}

void
VirtualLink::SetupTURN(
  const string & turn_server,
  const string & username,
  const std::string & password)
{
  if(turn_server.empty()) {
    LOG_F(LS_WARNING) << "No TURN Server address provided";
    return;
  }
  SocketAddress turn_addr;
  turn_addr.FromString(turn_server);
  LOG_F(LS_INFO) << "TURN " << turn_addr.ToString();
  cricket::RelayServerConfig relay_config_udp(cricket::RELAY_TURN);
  relay_config_udp.ports.push_back(cricket::ProtocolAddress(
    turn_addr, cricket::PROTO_UDP));
  if(username.empty() || password.empty())
    LOG_F(LS_WARNING) << "TURN credentials were not provided";
  relay_config_udp.credentials.username = username;
  relay_config_udp.credentials.password = password;
  if(!relay_config_udp.credentials.username.empty()) {
    port_allocator_->AddTurnServer(relay_config_udp);
  }

  //TODO - TCP relaying needs more testing
  //cricket::RelayServerConfig relay_config_tcp(cricket::RELAY_TURN);
  //relay_config_tcp.ports.push_back(cricket::ProtocolAddress(
  //  turn_addr, cricket::PROTO_TCP));
  //relay_config_tcp.credentials.username = username;
  //relay_config_tcp.credentials.password = password;
  //port_allocator_->AddTurnServer(relay_config_tcp);
}

void
VirtualLink::StartConnections()
{
  CreateCandidateConnections(vlink_desc_->cas);
}
} // end namespace tincan
