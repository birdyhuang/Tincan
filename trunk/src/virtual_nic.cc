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
#include "virtual_nic.h"
namespace tincan
{
VirtualNic::VirtualNic(VirtalNiConfig * vncfg) :
  tdev_(nullptr),
  config_(vncfg)
{
  tdev_ = new TapDev(
    make_unique<AsyncRead>(
      AsyncRead(make_unique<ReadCompletion>(
        ReadCompletion(fqr_)))),
    make_unique<AsyncWrite>(
      AsyncWrite((make_unique<WriteCompletion>(
        WriteCompletion(fqw_))))));
}

VirtualNic::~VirtualNic()
{
  delete config_;
  delete tdev_;
}
void VirtualNic::Configure()
{
  //initialize the Tap Device
  //tdev_->Open();
  //tdev_->SetIp4Addr();
  //tdev_->SetIp4Route();

  //Setup the vlink, aka webrtc connection
  //channel->SignalReadPacket.connect(
  //  this, &VirtualLink::OnReadPacket);

  //transport->SignalRequestSignaling.connect(
  //  this, &VirtualLink::OnRequestSignaling);

  //transport->SignalCandidatesReady.connect(
  //  this, &VirtualLink::OnCandidatesReady);

  //transport->SignalCandidatesAllocationDone.connect(
  //  this, &VirtualLink::OnCandidatesAllocationDone);

  //transport->SignalReadableState.connect(
  //  this, &VirtualLink::OnRWChangeState);

  //transport->SignalWritableState.connect(
  //  this, &VirtualLink::OnRWChangeState);

  //free the config as its not needed anymore
  delete config_;
  config_ = nullptr;
}
void VirtualNic::Start()
{
  tdev_->Up();
  tdev_->StartRead();
  vlink_thread_.Run();
}
void VirtualNic::Shutdown()
{
  tdev_->Down();
}

} //tincan
