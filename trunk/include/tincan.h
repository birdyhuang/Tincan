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

#include <memory>
#include <vector>
#include "virtual_network.h"
#include "control_listener.h"
#include "control_dispatch.h"

namespace tincan {
using namespace std;

class Tincan :
  public DispatchToTincanInf
{
public:
  Tincan();
  ~Tincan();
  void Start();
  void Shutdown();
  //
  //DispatchToTincanInf interface
  void CreateVNet(
    unique_ptr<LocalVnetEndpointConfig> lvecfg);
  
  void SetControllerHandle(
    ControllerHandle & ctrl_handle);
  
  void GetState(
    const string & tap_name,
    map<string, uint32_t>::const_iterator & it_begin,
    map<string, uint32_t>::const_iterator & it_end,
    Json::Value & state_data);

  void GetState(
    const string & tap_name,
    Json::Value & state_data);

  void SetIgnoredNetworkInterfaces(
    const string & tap_name,
    vector<string> & ignored_list);

private:
  void WaitForConfig();
  void WaitForExitSignal();
  VirtualNetwork & VnetByName(const string & tap_name);
  vector<unique_ptr<VirtualNetwork>> vnets_;
  ControlListener * ctrl_listener_;
  ControllerHandle * ctrl_handle_;
};
}
