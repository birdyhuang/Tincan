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
#include <map>
#include <vector>
using namespace std;
#include "vlink_uid_map.h"
#include "vnet_peer_map.h"
#include "remote_peer.h"
namespace tincan
{
enum MapLookupType
{
  IP4,
  IP6,
  MAC,
  UID,
  MAX_TYPE
};
class PeerNetwork
{
public:
  PeerNetwork();
	~PeerNetwork();
  RemotePeer & GetPeer(
    string identifier, 
    MapLookupType id_type);
private:
  //list of remote peer nodes on this virt net
  vector<RemotePeer> peers_;
  //map by ip4
  map<string, RemotePeer> ip4_map;
  //map by ip6
  map<string, RemotePeer> ip6_map;
  //map by mac
  map<string, RemotePeer> mac_map;
  //map by uid
  map<string, RemotePeer> uid_map;

};
} // namespace tincan


