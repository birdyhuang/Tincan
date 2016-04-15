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

Tincan::Tincan()
{}

Tincan::~Tincan()
{}

void
Tincan::Initialize()
{
  //Start tincan control to get config from Controller
  
  //...
  WaitForConfig();
  //parse config and create a vnic for each virtual interface that is specified
  for(auto & lvecfg : lve_cfglist) {
    auto vnet = make_unique<VirtualNetwork>(move(lvecfg));
    vnet->Configure();
    vnets_.push_back(move(vnet));
  }
}

void
Tincan::Start()
{
  for(auto const & vnet : vnets_) {
    vnet->Start();
  }
  WaitForExitSignal();
}

void
Tincan::Shutdown()
{
  for(auto const & vnet : vnets_) {
    vnet->Shutdown();
  }
}

void
Tincan::WaitForConfig()
{}

void
Tincan::WaitForExitSignal()
{}
} // namespace tincan

