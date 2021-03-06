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

#include "control_listener.h"
//#include "webrtc\base\thread.h"

namespace tincan
{
using namespace std;
using namespace rtc;

ControlListener::ControlListener(unique_ptr<ControlDispatch> control_dispatch) :
  control_dispatch_(move(control_dispatch)),
  socket_(nullptr),
  //socket6_(nullptr),
  //signal_thread_(nullptr),
  packet_options_(DSCP_DEFAULT)
{
  //signal_thread_ = Thread::Current();
  control_dispatch_->SetDispatchToListenerInf(this);
  BasicPacketSocketFactory packet_factory;
  //TODO: check for IPv6 capability
  socket_ = packet_factory.CreateUdpSocket(
    SocketAddress(params_.kLocalHost6, params_.kUdpPort), 0, 0);
  socket_->SignalReadPacket.connect(this, &ControlListener::ReadPacketHandler);
  //Use IPV4 if no IPv6 installed
  //socket_ = packet_factory.CreateUdpSocket(
  //  SocketAddress(params_.kLocalHost, tincan::kUdpPort), 0, 0);
 // socket_->SignalReadPacket.connect(this, &ControlListener::ReadPacketHandler);
}

ControlListener::~ControlListener()
{
  if(socket_) {
    delete socket_;
    socket_ = nullptr;
  }
}

void 
ControlListener::ReadPacketHandler(
  AsyncPacketSocket * socket,
  const char * data,
  size_t len,
  const SocketAddress & addr,
  const PacketTime & ptime)
{

  try {
    TincanControl ctrl(data, len);
    (*control_dispatch_)(ctrl);
  }
  catch(exception & e) {
    e.what();
  }
}
//
//ControllerHandle interface implementation
void ControlListener::Deliver(
  const string & uid,
  const string & type,
  const string & data)
{
  Json::Value json(Json::objectValue);
  json["uid"] = uid;
  json["data"] = data;
  json["type"] = type;
  std::string msg = json.toStyledString();
  //todo: add tincan header before sending to Controller
  Synchronized synch(skt_mutex_);
  socket_->SendTo(msg.c_str(), msg.length(), *controller_addr_.get(), packet_options_);
}

void ControlListener::Deliver(
  const char * packet,
  size_t packet_len)
{
  //todo: add tincan header before sending to Controller
  Synchronized synch(skt_mutex_);
  socket_->SendTo(packet, packet_len, *controller_addr_.get(), packet_options_);

}
// end ControllerHandle interface implementation
//

//
//DispatchtoListener interface implementation
void 
ControlListener::SetCtrlCb(
  unique_ptr<SocketAddress> controller_addr)
{
  controller_addr_ = move(controller_addr);
}
ControllerHandle & ControlListener::GetControllerHandle()
{
  return *this;
}
// end DispatchtoListener interface implementation
//
}  // namespace tincan
