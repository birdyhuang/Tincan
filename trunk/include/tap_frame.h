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
#ifndef TINCAN_TAPFRAME_H_
#define TINCAN_TAPFRAME_H_

#include <memory>

namespace tincan
{

using namespace std;
struct TapFrame
{
  TapFrame() : sz(0) {}

  explicit TapFrame(unsigned int frame_sz) :
    sz(frame_sz),
    buffer(make_unique<unsigned char[]>(frame_sz)) {}

  TapFrame(unsigned char * data, unsigned int len) : 
    sz(len), buffer(data)
  {}
  
  TapFrame(TapFrame & frame)
  {
    *this = frame;
  }
  
  TapFrame & operator= (const TapFrame & rhs)
  {
    if(this == &rhs) return *this;
    this->buffer = std::make_unique<unsigned char[]>(rhs.sz);
    memcpy(this->buffer.get(), rhs.buffer.get(), rhs.sz);
    this->sz = rhs.sz;
    return *this;
  }

  TapFrame &operator= (TapFrame && rhs)
  {
    if(this == &rhs) return *this;
    this->buffer = std::move(rhs.buffer);
    this->sz = rhs.sz;
    return *this;
  }

  unique_ptr<unsigned char[]>buffer;
  size_t sz;
};
}
#endif  // TINCAN_TAPFRAME_H_