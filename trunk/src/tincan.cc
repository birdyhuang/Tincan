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
#include "tincan.h"

namespace tincan
{

Tincan::Tincan() :
  ctrl_listener_(nullptr),
  ctrl_handle_(nullptr)
{}

Tincan::~Tincan()
{
  delete ctrl_listener_;
}

void
Tincan::Start()
{
  //Start tincan control to get config from Controller
  unique_ptr<ControlDispatch>ctrl_dispatch(new ControlDispatch);
  ctrl_dispatch->SetDispatchToTincanInf(this);
  ctrl_listener_ = new ControlListener(move(ctrl_dispatch));

  WaitForExitSignal();
}

void
Tincan::Shutdown()
{
  for(auto const & vnet : vnets_) {
    vnet->Shutdown();
  }
}

void Tincan::CreateVNet(unique_ptr<VnetDescriptor> lvecfg)
{  
  auto vnet = make_unique<VirtualNetwork>(move(lvecfg), *ctrl_handle_);
  vnet->Configure();
  vnets_.push_back(move(vnet));
  vnet->Start();
}

void 
Tincan::SetControllerHandle(
  ControllerHandle & ctrl_handle)
{
  ctrl_handle_ = &ctrl_handle;
}

void 
Tincan::GetState(
  const string & tap_name,
  map<string, uint32_t>::const_iterator & it_begin,
  map<string, uint32_t>::const_iterator & it_end,
  Json::Value & state_data)
{
  VirtualNetwork & vn = VnetFromName(tap_name);
  //TODO
}

void
Tincan::GetState(
  const string & tap_name,
  Json::Value & state_data)
{
  VirtualNetwork & vn = VnetFromName(tap_name);
  VnetDescriptor & lcfg = vn.LocalDescriptor();
  state_data["type"] = "local_state";
  state_data["uid"] = lcfg.uid;
  state_data["ip4"] = lcfg.vip4;
  state_data["ip6"] = lcfg.vip6;
  state_data["mac"] = vn.HWAddress();
  state_data["_fpr"] = vn.Fingerprint();
}

void 
Tincan::SetIgnoredNetworkInterfaces(
  const string & tap_name,
  vector<string>& ignored_list)
{
  VirtualNetwork & vn = VnetFromName(tap_name);
  vn.IgnoredNetworkInterfaces(ignored_list);
}

void
Tincan::CreateVnetLink(
  const Json::Value & link_desc)
{
  string tap_name = link_desc["tap_name"].asString();
  VirtualNetwork & vn = VnetFromName(tap_name);
  PeerDescriptor peer_desc;
  peer_desc.uid = link_desc["peer_uid"].asString();
  //TODO:
  peer_desc.vip4 = link_desc["vip4"].asString();
  peer_desc.vip6 = link_desc["vip6"].asString();
  unique_ptr<VlinkDescriptor> vl_desc;
  vl_desc->name = tap_name;
  vl_desc->overlay_id = link_desc["overlay_id"].asInt();
  vl_desc->peer_fpr = link_desc["peer_fpr"].asString();
  vl_desc->peer_uid = link_desc["peer_uid"].asString();
  vl_desc->sec_enabled = link_desc["sec_enabled"].asBool();
  vl_desc->stun_addr = link_desc["stun_addr"].asString();
  vl_desc->turn_addr = link_desc["turn_addr"].asString();
  vl_desc->turn_pass = link_desc["turn_pass"].asString();
  vl_desc->turn_user = link_desc["turn_user"].asString();

  vn.AddRemotePeer(peer_desc, move(vl_desc));
}

void
Tincan::RemoveVnetLink(
  const Json::Value & link_desc)
{
  const string & tap_name = link_desc["tap_name"].asString();
  VirtualNetwork & vn = VnetFromName(tap_name);
  const string & uid = link_desc["uid"].asString();
  vn.RemoveRemotePeer(uid);
}

void
Tincan::WaitForConfig()
{  //TODO
}

void
Tincan::WaitForExitSignal()
{  //TODO
}

VirtualNetwork & 
Tincan::VnetFromName(
  const string & tap_name)
{
  for(auto const & vnet : vnets_) {
    if(vnet->Name().compare(tap_name) == 0)//list of vnets will be small enough where a linear search is best
      return *vnet.get();
  }
  string msg("No virtual network exists by this name:");
  msg.append(tap_name);
  throw exception(msg.c_str());
}
} // namespace tincan

