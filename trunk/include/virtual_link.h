#ifndef _TINCAN_TINCAN_LINK_H_
#define _TINCAN_TINCAN_LINK_H_

#pragma warning( push )
#pragma warning(disable:4996)
#pragma warning(disable:4100)
#include "webrtc/p2p/base/p2ptransportchannel.h"
#include "webrtc/base/asyncpacketsocket.h"
#include "webrtc/p2p/base/p2ptransport.h"
#include "webrtc/p2p/client/basicportallocator.h"
#pragma warning( pop )
#include "vlink_events.h"

namespace tincan
{
class VirtualLink
{
public:
  VirtualLink();
  ~VirtualLink();
  void Initialize(VlinkEvents & vlink_events);

private:
  void SetupTransport();
  void CreateTransport();
  void RegisterLinkEventHandlers(VlinkEvents & vlink_events);

  unique_ptr<cricket::P2PTransport> transport_;
  unique_ptr<cricket::BasicPortAllocator> port_allocator_;
  unique_ptr<rtc::SSLFingerprint> remote_fingerprint_;
  unique_ptr<cricket::TransportDescription> local_description_;
  unique_ptr<cricket::TransportDescription> remote_description_;
  unique_ptr<cricket::P2PTransportChannel> channel_;
  unique_ptr<cricket::Candidates> candidates_;
  std::set<std::string> candidate_list_;

  shared_ptr<rtc::SSLFingerprint> local_fingerprint_;
  string connection_security_;
};

} //namespace tincan
#endif // !_TINCAN_TINCAN_LINK_H_
