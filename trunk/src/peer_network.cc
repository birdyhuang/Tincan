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
#include "peer_network.h"

namespace tincan
{
PeerNetwork::PeerNetwork(
  const string & name) :
  name_(name)
{}

PeerNetwork::~PeerNetwork()
{}

void PeerNetwork::Add(unique_ptr<RemotePeer> remote_peer)
{
  shared_ptr <RemotePeer> srp(remote_peer.release());
  //peers_.push_back(srp);
  ip4_map[srp->VirtIp4()] = srp;
  ip6_map[srp->VirtIp6()] = srp;
  uid_map[srp->Uid()] = srp;
  mac_map[srp->MacAddress()] = srp;
}

void PeerNetwork::Remove(
  const string & peer_uid)
{
  auto & rp = uid_map.find(peer_uid);
  if(rp != uid_map.end()) {
    ip4_map.erase(rp->second->VirtIp4());
    ip6_map.erase(rp->second->VirtIp6());
    mac_map.erase(rp->second->MacAddress());
    uid_map.erase(rp);
  }
  else
    LOG_F(LS_INFO) << "Failed to peer with UID:" << peer_uid 
    << "from peer network: " << name_ << ". It was not found";
}

RemotePeer &
PeerNetwork::UidToPeer(
  const string & id) const
{
  return *uid_map.at(id).get();
}

RemotePeer &
PeerNetwork::Ip4ToPeer(
  const string & ip4) const
{
  return *ip4_map.at(ip4).get();
}

RemotePeer &
PeerNetwork::Ip6ToPeer(
  const string & ip6) const
{
  return *ip6_map.at(ip6).get();
}

RemotePeer &
PeerNetwork::MacAddressToPeer(
  const string & mac) const
{
  return *mac_map.at(mac).get();
}
} // namespace tincan
