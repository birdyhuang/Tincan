#ifndef _TINCAN_TINCAN_LINK_H_
#define _TINCAN_TINCAN_LINK_H_

#pragma warning( push )
#pragma warning(disable:4996)
#pragma warning(disable:4100)
#include "webrtc/base/asyncpacketsocket.h"
#include "webrtc/base/sslfingerprint.h"
#include "webrtc/base/network.h"
#include "webrtc/base/thread.h"
#include "webrtc/p2p/base/p2ptransport.h"
#include "webrtc/p2p/base/dtlstransport.h"
#include "webrtc/p2p/base/p2ptransportchannel.h"
#include "webrtc/p2p/base/basicpacketsocketfactory.h"
#include "webrtc/p2p/client/basicportallocator.h"
//#include "webrtc/p2p/client/httpportallocator.h"
#pragma warning( pop )
//#include "vlink_events.h"
#include "tap_frame.h"
#include "tincan_parameters.h"

namespace tincan
{
typedef cricket::DtlsTransport<cricket::P2PTransport> DtlsP2PTransport;

class FrameHandler
{
public:
  virtual void ReceiveFrame(TapFrame & frame) = 0;
  virtual void SendFrame(TapFrame & frame) = 0;
};

struct VlinkDescriptor
{
  bool sec_enabled;
  int overlay_id;
  string name;
  string peer_uid;
  string peer_fpr;
  string stun_addr;
  string turn_addr;
  string turn_user;
  string turn_pass;
  string cas;
};

class VirtualLink : public sigslot::has_slots<>
{
public:
  VirtualLink(
    unique_ptr<const VlinkDescriptor> vlink_desc,
    FrameHandler & FrameRcvHandler);
  ~VirtualLink();
  
  void Initialize(
   // VlinkEvents & vlink_events,
    //const VlinkDescriptor & vlink_desc,
    const string & local_uid,
    rtc::BasicNetworkManager & network_manager,
    const rtc::SSLFingerprint & local_fingerprint,
    const rtc::SSLIdentity & sslid);

  void StartConnections();

  void Transmit(TapFrame & frame);

  //void LocalFingerprint(
  //  const rtc::SSLFingerprint & local_fingerprint);

private:
  void CreateTransport(
    //const VlinkDescriptor & vlink_cfg,
    rtc::BasicNetworkManager & network_manager,
    const rtc::SSLFingerprint & local_fingerprint,
    const rtc::SSLIdentity & sslid);

  void SetupTURN(
    const string & turn_server,
    const string & username,
    const std::string & password);

  void RegisterLinkEventHandlers();

  void CreateCandidateConnections(
    const string & candidates);
  
  void SetupTransport(
    const string & local_uid,
    const rtc::SSLFingerprint & local_fingerprint,
    const string & peer_fpr);

  void OnReadPacket(
    cricket::TransportChannel* channel,
    const char* data,
    size_t len,
    const rtc::PacketTime & ptime,
    int flags);

  unique_ptr<const VlinkDescriptor> vlink_desc_;
  rtc::BasicPacketSocketFactory packet_factory_;
  unique_ptr<cricket::P2PTransport> transport_;
  unique_ptr<cricket::BasicPortAllocator> port_allocator_;
  //unique_ptr<cricket::HttpPortAllocator> port_allocator_;
  unique_ptr<cricket::P2PTransportChannel> channel_;
  unique_ptr<cricket::Candidates> candidates_;
  const uint64_t tiebreaker_;

  unique_ptr<cricket::TransportDescription> local_description_;
  unique_ptr<cricket::TransportDescription> remote_description_;

  //rtc::Thread packet_handling_thread;
  //rtc::Thread link_setup_thread;
  unique_ptr<rtc::SSLFingerprint> remote_fingerprint_;
  string connection_security_;
  TincanParameters params_;
  const string & content_name_;
  FrameHandler & FrameRcvHandler_;
};

} //namespace tincan
#endif // !_TINCAN_TINCAN_LINK_H_
