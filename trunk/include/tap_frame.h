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
#include <string>
#include "tincan_parameters.h"
namespace tincan
{
using namespace std;
class TapFrame
{
  friend class FrameProperties;
public:
  TapFrame() :
    fd_capacity_(TincanParameters::kBufferSize),
    hdr_capacity_(TincanParameters::kFrameHeaderSize),
    data_(new uint8_t[hdr_capacity_ + fd_capacity_])
  {}

  TapFrame(const TapFrame & frame)
  {}

  TapFrame(TapFrame && frame)
  {}

  //creates frame + header
  explicit TapFrame(size_t frame_sz)
  {}

  TapFrame(unsigned char * data, size_t len)
  {}

  TapFrame & operator= (const TapFrame & rhs)
  {}

  TapFrame &operator= (TapFrame && rhs)
  {}

  bool operator==(const TapFrame & frame) const
  {}

  bool operator!=(const TapFrame & frame) const
  {
    return !(*this == frame);
  }

  uint8_t operator[](size_t index)
  {}

  void SetSourceUid(const string & src_uid)
  {}

  void SetDestinationUid(const string & dest_uid)
  {}

  void SetFrameData(uint8_t frm_data, size_t len)
  {}
  string & SourceUid()
  {}

  string & DestinationUid()
  {}

  uint8_t * Header()
  {}

  uint8_t * Data();

  size_t Capacity();

  void SwapUids()
  {}

private:
  size_t hdr_capacity_;
  size_t fd_sz_;
  size_t fd_capacity_;
  unique_ptr<uint8_t[]> data_;
  uint8_t itr_;

};

class FrameProperties{
public:
  FrameProperties(TapFrame & frame) :
    frame_(frame)
  {}

  bool IsIccMsg() const;

  bool IsIp4() const;

  bool IsIp6() const;

  bool IsArpRequest() const;

  bool IsArpResponse() const;

  bool IsUnicast() const;

  bool IsBroadcast() const;


private:
  const TapFrame & frame_;
};

}
#endif  // TINCAN_TAPFRAME_H_
