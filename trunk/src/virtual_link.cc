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
#include<memory>
#include "tap_frame.h"
#include "virtual_link.h"
#include "webrtc/p2p/base/dtlstransport.h"

namespace tincan
{
using namespace std;

static const size_t kFlags = 0;
static const char kFprNull[] =
"00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";

VirtualLink::VirtualLink()
{}

VirtualLink::~VirtualLink()
{}

void VirtualLink::Initialize(VlinkEvents & vlink_events)
{

  CreateTransport();
  RegisterLinkEventHandlers(vlink_events);
  SetupTransport();
}

void VirtualLink::CreateTransport()
{
   std::string uid;  std::string fingerprint; int overlay_id;
  std::string stun_server;  std::string turn_server;
  std::string turn_user;  std::string turn_pass;
  bool sec_enabled = true;
  
  rtc::SocketAddress stun_addr;
  stun_addr.FromString(stun_server);
  //port_allocator.reset(new cricket::BasicPortAllocator(
  //  &network_manager_, &packet_factory_, stun_addr));
  //port_allocator->set_flags(kFlags);
  //SetRelay(peer_state.get(), turn_server, turn_user, turn_pass);

  int component = cricket::ICE_CANDIDATE_COMPONENT_DEFAULT;
  if(sec_enabled && local_fingerprint_ &&
    fingerprint.compare(kFprNull) != 0) {
    //transport = make_unique<DtlsP2PTransport>(link_setup_thread_, packet_handling_thread_,
    //  content_name_, peer_state->port_allocator.get(), identity_.get());

    //cricket::DtlsTransportChannelWrapper* dtls_channel =
    //  static_cast<cricket::DtlsTransportChannelWrapper*>(
    //    transport->CreateChannel(component));

    //channel_ = static_cast<cricket::P2PTransportChannel*>(
    //  dtls_channel->channel());
    //connection_security_ = "dtls";
  }
  else {
    //transport_.reset(new cricket::P2PTransport(
    //  link_setup_thread_, packet_handling_thread_, content_name_,
    //  peer_state->port_allocator.get()));
    //channel_ = static_cast<cricket::P2PTransportChannel*>(
    //  transport->CreateChannel(component););
    //connection_security_ = "none";
  }

}

//bool VirtualLink::SetRelay(
//  PeerState* peer_state, const std::string& turn_server,
//  const std::string& username, const std::string& password)
//{
//  if(turn_server.empty() || username.empty()) return false;
//  talk_base::SocketAddress turn_addr;
//  turn_addr.FromString(turn_server);
//  cricket::RelayServerConfig relay_config_udp(cricket::RELAY_TURN);
//  cricket::RelayServerConfig relay_config_tcp(cricket::RELAY_TURN);
//  relay_config_udp.ports.push_back(cricket::ProtocolAddress(
//    turn_addr, cricket::PROTO_UDP));
//  relay_config_udp.credentials.username = username;
//  relay_config_udp.credentials.password = password;
//  relay_config_tcp.ports.push_back(cricket::ProtocolAddress(
//    turn_addr, cricket::PROTO_TCP));
//  relay_config_tcp.credentials.username = username;
//  relay_config_tcp.credentials.password = password;
//  if(!relay_config_udp.credentials.username.empty()) {
//    peer_state->port_allocator->AddRelay(relay_config_udp);
//    // TODO - TCP relaying needs more testing
//    //peer_state->port_allocator->AddRelay(relay_config_tcp);
//  }
//  LOG_TS(INFO) << "TURN " << turn_addr.ToString();
//  return true;
//}

}
