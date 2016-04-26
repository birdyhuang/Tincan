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
#include "remote_peer.h"

namespace tincan
{
RemotePeer::RemotePeer(
  PeerDescriptor const & descriptor,
  TapDev & tap_dev):
  tap_(tap_dev)
{}

RemotePeer::~RemotePeer()
{}

void
RemotePeer::SetVirtualLink(
  unique_ptr<VirtualLink> vlink)
{
  vlink_ = move(vlink);
  vlink_->StartConnections();
}

void RemotePeer::TrimLink()
{
  vlink_.reset();
}
void RemotePeer::SendFrame(TapFrame & frame)
{
  //TODO: tunneling
  vlink_->Transmit(frame);
}
void RemotePeer::ReceiveFrame(TapFrame & frame)
{
  //TODO: tunneling
  tap_.Write(frame);
}
} // namespace tincan
