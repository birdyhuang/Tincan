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
#ifndef TINCAN_CONTROL_LISTENER_H_
#define TINCAN_CONTROL_LISTENER_H_

#include "controller_handle.h"
#include <memory>
#pragma warning(disable:4996)
#include "webrtc/base/socketaddress.h"
#include "webrtc/p2p/base/basicpacketsocketfactory.h"
#include "webrtc/base/logging.h"
#include "webrtc/base/sigslot.h"
#include "webrtc/base/asyncpacketsocket.h"
#pragma warning(default:4996)
#include "control_dispatch.h"
namespace tincan
{
using namespace std;
using namespace rtc;
class ControlListener : 
  public ControllerHandle,
  public DispatchToListenerInf,
  public sigslot::has_slots<>
{
public:
  ControlListener(unique_ptr<ControlDispatch> control_dispatch);
  ~ControlListener();
  void ReadPacketHandler(
    AsyncPacketSocket * socket,
    const char * data,
    size_t len,
    const SocketAddress & addr,
    const PacketTime & ptime);

  //ControllerHandle implementation
  void Deliver(
    const char * packet,
    size_t packet_len);

  void Deliver(
    const string & uid,
    const string & type,
    const string & msg);

  //Dispatch to Listener Interface Implementation
  void SetCtrlCb(
    unique_ptr<SocketAddress> controller_addr
    );
  ControllerHandle & GetControllerHandle();
private:
  unique_ptr<ControlDispatch> control_dispatch_;
  AsyncPacketSocket * socket_;
  unique_ptr<SocketAddress> controller_addr_;
  //AsyncPacketSocket * socket6_;
  //Thread *signal_thread_;
  PacketOptions packet_options_;

};
}  // namespace tincan
#endif  // TINCAN_CONTROL_LISTENER_H_
