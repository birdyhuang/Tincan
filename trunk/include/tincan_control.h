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
#ifndef TINCAN_TINCAN_CONTROL_H_
#define TINCAN_TINCAN_CONTROL_H_
#include <memory>
#include <string>
#include "webrtc/base/json.h"
#pragma warning(disable:4996)
#include "webrtc/base/socketaddress.h"
#pragma warning(default:4996)
#include "controller_handle.h"
#include "vnet_descriptor.h"

namespace tincan {
using namespace std;
using namespace rtc;

class DispatchToListenerInf
{
public:
  virtual void SetCtrlCb(
    unique_ptr<SocketAddress> controller_addr) = 0;
  virtual ControllerHandle & GetControllerHandle() = 0;
};

class DispatchToTincanInf
{
public:
  virtual void CreateVNet(
    unique_ptr<VnetDescriptor> lvecfg) = 0;
  
  virtual void SetControllerHandle(
    ControllerHandle & ctrl_handle) = 0;
  
  virtual void GetState(
    const string & tap_name,
    map<string, uint32_t>::const_iterator & it_begin,
    map<string, uint32_t>::const_iterator & it_end,
    Json::Value & state_data) = 0;

  virtual void GetState(
    const string & tap_name,
    Json::Value & state_data) = 0;

  virtual void SetIgnoredNetworkInterfaces(
    const string & tap_name,
    vector<string> & ignored_list) = 0;

  virtual void CreateVnetLink(
    const Json::Value & link_desc) = 0;

  virtual void RemoveVnetLink(
    const Json::Value & link_desc) = 0;

};

class TincanControl
{
public:
  TincanControl(const char * const data, const size_t len);
  ~TincanControl();
  string Name() const;
  const unsigned char Type() const;
  void AsJson(Json::Value & dict);
private:
  string message_;
  const char * const data_;
  const size_t data_len_;
  size_t INDEX_VERSION = 0;
  size_t INDEX_TYPE = 1;
};
} // namespace tincan
#endif  // TINCAN_TINCAN_CONTROL_H_