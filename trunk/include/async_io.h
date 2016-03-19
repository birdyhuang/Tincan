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

#include <windows.h>
#include "tap_frame.h"
#include "frame_queue.h"

namespace tincan
{
template<typename T>
struct AsyncIo : public OVERLAPPED
{
  HANDLE dev_handle;
  TapFrame frame;
  LPOVERLAPPED_COMPLETION_ROUTINE io_cmpl_routine;
 ? T *completion;
};

struct ReadCompletion
{
  ReadCompletion(FrameQueue & iframe_queue) : iframe_queue(iframe_queue) {}
  int operator()(TapFrame & frame)
  {
    iframe_queue.push(frame);
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
  }
  FrameQueue & oframe_queue;
};

struct AsyncIoCompletion
{
  AsyncIoCompletion():
    //ReadCompletion & read_compl,
    //WriteCompletion & write_compl) :
    //read_completion(read_compl),
    //write_completion(write_compl),
    dev_handle(INVALID_HANDLE_VALUE)
  {}
  ~AsyncIoCompletion()
  {
    if(INVALID_HANDLE_VALUE != dev_handle)
      CloseHandle(dev_handle);
  }
  void SetDevHandle(HANDLE handle)
  {
    dev_handle = handle;
    read_overlap.dev_handle = handle;
    write_overlap.dev_handle = handle;
  }
  HANDLE dev_handle;
  AsyncIo<ReadCompletion> read_overlap;
  AsyncIo<WriteCompletion> write_overlap;
  //ReadCompletion & read_completion;
  //WriteCompletion & write_completion;
};


}
#endif// TINCAN_ASYNCIO_H_