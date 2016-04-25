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
#ifndef TINCAN_CONTROL_DISPATCH_H_
#define TINCAN_CONTROL_DISPATCH_H_
#include <map>
#include <memory>
#include "tincan_control.h"
namespace tincan
{
using namespace std;
enum TincanCtrls
{
  REGISTER_SVC = 1,
  CREATE_LINK = 2,
  SET_LOCAL_IP = 3,
  SET_REMOTE_IP = 4,
  TRIM_LINK = 5,
  SET_CB_ENDPOINT = 6,
  GET_STATE = 7,
  SET_LOGGING = 8,
  SET_TRANSLATION = 9,
  SET_SWITCHMODE = 10,
  SET_TRIMPOLICY = 11,
  ECHO_REQUEST = 12,
  ECHO_REPLY = 13,
  SET_NETWORK_IGNORE_LIST = 14,
  MAX_OPS,
};
class ControlDispatch
{
public:
  ControlDispatch();
  ~ControlDispatch();
  void operator () (TincanControl & control);
  void SetDispatchToTincanInf(DispatchToTincanInf * dtot);
  void SetDispatchToListenerInf(DispatchToListenerInf * dtol);

private:
  void RegisterService(TincanControl & control);
  void CreateLink(TincanControl & control);
  //void SetLocalIp(TincanControl & control);
  //void SetRemoteIp(TincanControl & control);
  void TrimLink(TincanControl & control);
  void CreateCtrlEndpoint(TincanControl & control);
  void GetState(TincanControl & control);
  void SetLogLevel(TincanControl & control);
  //void SetTranslation(TincanControl & control);
  //void SetSwitchmode(TincanControl & control);
  //void SetTrimPolicy(TincanControl & control);
  void EchoRequest(TincanControl & control);
  //void EchoReply(TincanControl & control);
  void SetNetworkIgnoreList(TincanControl & control);
  void SendICC(TincanControl & control);
  void CreateVNet(TincanControl & control);

//  static map<std::string, int>control_map;
  map<std::string, void (ControlDispatch::*)(TincanControl & control)>control_map;
  DispatchToListenerInf * dtol_;
  DispatchToTincanInf * dtot_;
  ControllerHandle * ctrl_handle_;
};
}  // namespace tincan
#endif  // TINCAN_CONTROL_DISPATCH_H_
