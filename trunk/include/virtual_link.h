#ifndef _TINCAN_TINCAN_LINK_H_
#define _TINCAN_TINCAN_LINK_H_

#include "webrtc/p2p/base/transportchannel.h"
#include "webrtc/base/asyncpacketsocket.h"
namespace tincan
{
class VirtualLink
{
public:
  VirtualLink();
  ~VirtualLink();
  void OnReadPacket(
    cricket::TransportChannel * channel,
    const char* data,
    size_t len,
    const rtc::PacketTime & ptime,
    int flags);
private:
  //talk_base::scoped_ptr<cricket::P2PTransport> transport;
  //talk_base::scoped_ptr<cricket::BasicPortAllocator> port_allocator;
  //talk_base::scoped_ptr<talk_base::SSLFingerprint> remote_fingerprint;
  //talk_base::scoped_ptr<cricket::TransportDescription> local_description;
  //talk_base::scoped_ptr<cricket::TransportDescription> remote_description;
  //cricket::P2PTransportChannel* channel;
  //cricket::Candidates candidates;
  //std::set<std::string> candidate_list;
};

} //namespace tincan
#endif // !_TINCAN_TINCAN_LINK_H_
