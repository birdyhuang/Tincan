#ifndef _TINCAN_TINCAN_LINK_H_
#define _TINCAN_TINCAN_LINK_H_

#pragma warning( push )
#pragma warning(disable:4996)
#pragma warning(disable:4100)
#pragma warning(disable:4459)
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
#include "tap_frame.h"
#include "tincan_parameters.h"

namespace tincan
{
using DtlsP2PTransport = cricket::DtlsTransport<cricket::P2PTransport>;

/*
Interface FrameHandler specifies the symantics for reading and writing of an
IPOP TAP frame from the perspective of a remote peer. This is where the 
tunneling behavior would be defined, in the class that realizes this interface.
The functions ProcessOutgoingFrame() and ProcessIncomingFrame(), is to be
interpreted w.r.t to the local virtual network. Consequently
ProcessIncomingFrame is the request used when a frame is read from the remote
peer via the virtual link (aka tincan link), to perform vnet tunneling and
write the frame to the TAP device. ProcessOutgoingFrame is the request used to
send a frame, which was read from the TAP device, to the remote peer via its
virtual link.
*/
class VirtualLink;
//template<typename _VLINK>
class FrameHandler
{
public:
  virtual void ProcessOutgoingFrame(
    TapFrame & frame, VirtualLink & vlink) = 0;

  virtual void SwitchmodeProcessOutgoingFrame(
    TapFrame & frame, VirtualLink & vlink) = 0;

    virtual void ProcessIncomingFrame(
      TapFrame & frame, VirtualLink & vlink) = 0;

    virtual void SwitchmodeProcessIncomingFrame(
      TapFrame & frame, VirtualLink & vlink) = 0;
};

//template<typename _VLINK>
struct IncomingFrameHandler
{
  IncomingFrameHandler(
    FrameHandler & fh,
    void (FrameHandler::*ProcessIncomingFrame)(
      TapFrame & frame, VirtualLink & vlink)) :
    fh_(fh),
    ProcessIncomingFrame_(ProcessIncomingFrame)
  {}
  void operator()(
    TapFrame & frame, VirtualLink & vlink)
  {
    (fh_.*ProcessIncomingFrame_)(frame, vlink);
  }

  FrameHandler & fh_;
  void (FrameHandler::*ProcessIncomingFrame_)(
    TapFrame & frame, VirtualLink & vlink);
};

struct  VlinkDescriptor
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
    unique_ptr<IncomingFrameHandler> ifh);
  ~VirtualLink();

  void Initialize(
    const string & local_uid,
    rtc::BasicNetworkManager & network_manager,
    const rtc::SSLFingerprint & local_fingerprint,
    const rtc::SSLIdentity & sslid);

  void StartConnections();

  void Transmit(TapFrame & frame);

private:
  void CreateTransport(
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
  unique_ptr<IncomingFrameHandler> iframehandler_;
};
} //namespace tincan
#endif // !_TINCAN_TINCAN_LINK_H_
