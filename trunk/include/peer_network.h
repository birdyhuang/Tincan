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
#include <memory>
#include <vector>
using namespace std;
#include "remote_peer.h"
namespace tincan
{
class PeerNetwork
{
public:
  PeerNetwork(
    const string & name);
	~PeerNetwork();
  void Add(unique_ptr<RemotePeer> remote_peer);
  void Remove(const string & peer_uid);
  
  RemotePeer & UidToPeer(const string & id);
private:
  const string & name_;
  //list of remote peer nodes on this virt net
  //vector<shared_ptr<RemotePeer>> peers_;
  //map by ip4
  map<string, shared_ptr<RemotePeer>> ip4_map;
  //map by ip6
  map<string, shared_ptr<RemotePeer>> ip6_map;
  //map by mac
  map<string, shared_ptr<RemotePeer>> mac_map;
  //map by uid
  map<string, shared_ptr<RemotePeer>> uid_map;

};
} // namespace tincan


