#ifndef _TINCAN_TINCAN_LINK_H_
#define _TINCAN_TINCAN_LINK_H_

#include "p2p/base/transportchannel.h"
#include "base/asyncpacketsocket.h"
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

};

} //namespace tincan
#endif // !_TINCAN_TINCAN_LINK_H_
