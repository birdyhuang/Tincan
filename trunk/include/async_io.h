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
#ifndef TINCAN_ASYNCIO_H_
#define TINCAN_ASYNCIO_H_

#include <memory>
#include "tap_frame.h"
#include "frame_queue.h"

namespace tincan
{

struct ReadCompletion
{
  ReadCompletion(FrameQueue & iframe_queue) : iframe_queue(iframe_queue) {}
  int operator()(TapFrame & frame)
  {
    //iframe_queue.push(frame);
    return 0;
  }
  FrameQueue & iframe_queue;
};

struct WriteCompletion
{
  WriteCompletion(FrameQueue & oframe_queue) : oframe_queue(oframe_queue) {}
  int operator()(TapFrame & frame)
  {
    oframe_queue.front();
    oframe_queue.pop();
    return 0;
  }
  FrameQueue & oframe_queue;
};

struct AsyncRead
#if defined(_IPOP_WIN)
: public OVERLAPPED
#endif // defined(_IPOP_WIN)
{
  AsyncRead(unique_ptr<ReadCompletion> rd_cmpl) : completion(std::move(rd_cmpl)) {}
  HANDLE dev_handle;
  TapFrame frame;
  unique_ptr<ReadCompletion> completion;
};

struct AsyncWrite
#if defined(_IPOP_WIN)
: public OVERLAPPED
#endif // defined(_IPOP_WIN)
{
  AsyncWrite(unique_ptr<WriteCompletion> wr_cmpl) : completion(std::move(wr_cmpl)) {}
  HANDLE dev_handle;
  TapFrame frame;
  unique_ptr<WriteCompletion> completion;
};

struct AsyncIo
{
  AsyncIo(
    unique_ptr<ReadCompletion> read_compl,
    unique_ptr<WriteCompletion> write_compl) :
    asyncrd(std::move(read_compl)),
    asyncwr(std::move(write_compl))
  {}

  AsyncRead asyncrd;
  AsyncWrite asyncwr;
};


}
#endif// TINCAN_ASYNCIO_H_