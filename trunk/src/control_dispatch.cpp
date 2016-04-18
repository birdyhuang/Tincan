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

#include "control_dispatch.h"
#include <map>
#include <string>
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"

namespace tincan
{
using namespace std;
using namespace rtc;

//map<string, int> ControlDispatch::control_map{ 
//  { "register_svc", REGISTER_SVC },
//  { "create_link", CREATE_LINK },
//  { "set_local_ip", SET_LOCAL_IP },
//  { "set_remote_ip", SET_REMOTE_IP },
//  { "trim_link", TRIM_LINK },
//  { "set_cb_endpoint", SET_CB_ENDPOINT },
//  { "get_state", GET_STATE },
//  { "set_logging", SET_LOGGING },
//  { "set_translation", SET_TRANSLATION },
//  { "set_switchmode", SET_SWITCHMODE },
//  { "set_trimpolicy", SET_TRIMPOLICY },
//  { "echo_request", ECHO_REQUEST },
//  { "echo_reply", ECHO_REPLY },
//  { "set_network_ignore_list", SET_NETWORK_IGNORE_LIST }
//};
ControlDispatch::ControlDispatch() :
  dtol_(nullptr)
{
  map<string, void (ControlDispatch::*)(TincanControl & control)> control_map2 = {
  { "register_svc", &ControlDispatch::RegisterService },
  { "create_link", &ControlDispatch::CreateLink },
  { "set_local_ip", &ControlDispatch::SetLocalIp },
  { "set_remote_ip", &ControlDispatch::SetRemoteIp },
  { "trim_link", &ControlDispatch::TrimLink },
  { "set_cb_endpoint", &ControlDispatch::SetCbEndpoint },
  { "get_state", &ControlDispatch::GetState },
  { "set_logging", &ControlDispatch::SetLogLevel },
  { "set_translation", &ControlDispatch::SetTranslation },
  { "set_switchmode", &ControlDispatch::SetSwitchmode },
  { "set_trimpolicy", &ControlDispatch::SetTrimPolicy },
  { "echo_request", &ControlDispatch::EchoRequest },
  { "echo_reply", &ControlDispatch::EchoReply },
  { "set_network_ignore_list", &ControlDispatch::SetNetworkIgnoreList }
};
}

ControlDispatch::~ControlDispatch()
{}

void
ControlDispatch::operator () (TincanControl & control)
{
  // todo:
  const int kTincanPacket = 0, kICCControl = 1, kTincanControl = 2;
  switch(control.Type()) {
  case kTincanPacket:
    // todo: ProcessIPPacket(socket, data, len, addr);
    break;
  case kICCControl:
    /* ICC message is received from controller. Remove IPOP version and type
    field and pass to TinCan Connection manager */
    // todo: manager_.DoPacketSend(data + 2, len - 2);
    break;
  case kTincanControl:
    (this->*control_map2[control.Name()])(control);
    break;
  default:
    throw exception("Invalid Tincan control type received");
  }
  //void (ControlDispatch::*CtrlHandler)() = control_map2[control.Name()];
  //(this->*CtrlHandler)();


  //switch(control_map[control.Name()]) {
  //case REGISTER_SVC:
  //  RegisterService();
  //  break;
  //case CREATE_LINK:
  //  CreateLink();
  //  break;
  //case SET_LOCAL_IP:
  //  SetLocalIp();
  //  break;
  //case SET_REMOTE_IP:
  //  SetRemoteIp();
  //  break;
  //case TRIM_LINK:
  //  TrimLink();
  //  break;
  //case SET_CB_ENDPOINT:
  //  SetCbEndpoint();
  //  break;
  //case GET_STATE:
  //  GetState();
  //  break;
  //case SET_LOGGING:
  //  SetLogLevel();
  //  break;
  //case SET_TRANSLATION:
  //  SetTranslation();
  //  break;
  //case SET_SWITCHMODE:
  //  SetSwitchmode();
  //  break;
  //case SET_TRIMPOLICY:
  //  SetTrimPolicy();
  //  break;
  //case ECHO_REQUEST:
  //  EchoRequest();
  //  break;
  //case ECHO_REPLY:
  //  EchoReply();
  //  break;
  //case SET_NETWORK_IGNORE_LIST:
  //  SetNetworkIgnoreList();
  //  break;
  //default:
  //  //LOG_TS(WARN) << "Unrecognized Tincan Control received" << control.Body();
  //  break;

  //}
}
void 
ControlDispatch::SetDispatchToTincanInf(
  DispatchToTincanInf * dtot)
{
  dtot_ = dtot;
}
void 
ControlDispatch::SetDispatchToListenerInf(
  DispatchToListenerInf * dtol)
{
  dtol_ = dtol;
}
void ControlDispatch::RegisterService(TincanControl & control)
{}
void 
ControlDispatch::CreateLink(
  TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  int overlay_id = req["overlay_id"].asInt();
  std::string uid = req["uid"].asString();
  std::string fpr = req["fpr"].asString();
  std::string stun = req["stun"].asString();
  std::string turn = req["turn"].asString();
  std::string turn_user = req["turn_user"].asString();
  std::string turn_pass = req["turn_pass"].asString();
  std::string cas = req["cas"].asString();
  bool sec = req["sec"].asBool();
  //todo: Create Link
}
void 
ControlDispatch::SetLocalIp(
  TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string uid = req["uid"].asString();
  std::string ip4 = req["ip4"].asString();
  std::string ip6 = req["ip6"].asString();
  int ip4_mask = req["ip4_mask"].asInt();
  int ip6_mask = req["ip6_mask"].asInt();
  int subnet_mask = req["subnet_mask"].asInt();
  int switchmode = req["switchmode"].asInt();
  //TODO: set IP
}
void ControlDispatch::SetRemoteIp(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string uid = req["uid"].asString();
  std::string ip4 = req["ip4"].asString();
  std::string ip6 = req["ip6"].asString();
  //TODO: set remote ip
}
void ControlDispatch::TrimLink(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string uid = req["uid"].asString();
  //todo
}

void ControlDispatch::SetCbEndpoint(
  TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string ip = req["ip"].asString();
  int port = req["port"].asInt();
  unique_ptr<SocketAddress> ctrl_addr(new SocketAddress(ip, port));
  dtol_->SetCtrlCb(move(ctrl_addr));
}

void ControlDispatch::GetState(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string uid = req["uid"].asString();
  bool get_stats = req["stats"].asBool();
  //todo: SendState(uid, get_stats, addr);
}

void ControlDispatch::SetLogLevel(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  int logging = req["logging"].asInt();
  if(logging == 0) {
    rtc::LogMessage::LogToDebug(rtc::LS_ERROR);
  }
  else if(logging == 1) {
    rtc::LogMessage::LogToDebug(rtc::LS_WARNING);
  }
  else if(logging == 2) {
    rtc::LogMessage::LogToDebug(rtc::LS_INFO);
  }
  else if(logging == 3) {
    rtc::LogMessage::LogToDebug(rtc::LS_SENSITIVE);
  }

}
void ControlDispatch::SetTranslation(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  int translate = req["translate"].asInt();
  //todo
}
void ControlDispatch::SetSwitchmode(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  int switchmode = req["switchmode"].asInt();
  //todo
}
void ControlDispatch::SetTrimPolicy(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  bool trim = req["trim_enabled"].asBool();
  //todo
}
void ControlDispatch::EchoRequest(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  std::string msg = req["msg"].asString();
  Json::Value local_state;
  local_state["type"] = "echo_request";
  local_state["msg"] = msg;
  req = local_state.toStyledString();
  //todo: SendTo(req.c_str(), req.size(), addr);
}
void ControlDispatch::EchoReply(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
}
void ControlDispatch::SetNetworkIgnoreList(TincanControl & control)
{
  Json::Value req;
  control.AsJson(req);
  int count = req["network_ignore_list"].size();
  Json::Value network_ignore_list = req["network_ignore_list"];
  //LOG_TS(INFO) << "Listed network device is ignored for TinCan connection"
  //  << network_ignore_list.toStyledString();
  std::vector<std::string> ignore_list(count);
  for(int i = 0; i<count; i++) {
    ignore_list[i] = network_ignore_list[i].asString();
  }
  //todo: manager_.set_network_ignore_list(ignore_list);
}

void 
ControlDispatch::CreateVNet(
  TincanControl & control)
{
  Json::Value dict;
  control.AsJson(dict);
  unique_ptr<LocalVnetEndpointConfig> cfg(new LocalVnetEndpointConfig);
  cfg->real_ip = dict["real_ip"].asString();
  cfg->tap_name = dict["tap_name"].asString();
  cfg->uid = dict["uid"].asString();
  cfg->vip4 = dict["vip4"].asString();
  cfg->vip6 = dict["vip6"].asString();
  cfg->xmpp_pw = dict["xmpp_pw"].asString();
  cfg->xmpp_url = dict["xmpp_url"].asString();
  cfg->xmpp_user = dict["xmpp_user"].asString();
  cfg->switchmode = dict["xmpp_user"].asBool();
  cfg->trim_enabled = dict["trim_enabled"].asBool();
  cfg->translate = dict["translate"].asInt();
  dtot_->CreateVNet(move(cfg));
}
}  // namespace tincan
