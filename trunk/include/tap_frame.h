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
#include <cstring>
#include "tincan_exception.h"
#include "tincan_parameters.h"
namespace tincan
{
using namespace std;

/*
The TapFrameBuffer (TFB) is the byte container for a tincan frame's data. This
includes the tincan specific headers as well as the payload data received from
the TAP device or tincan link. A TFB employes single owner sematics and facilitates
decoupling of the raw data from the meta-data required to manage it.
*/
class TapFrameBuffer
{
  friend class TapFrame;
  friend class FrameBufferCache;
  TapFrameBuffer() = default;
  ~TapFrameBuffer() = default;
  char src_id[20];
  char trg_id[20];
  uint8_t payload[1024];// TincanParameters::kBufferSize];
};

/*
Holds a fixed pool of TFBs and acts as a custom allocator for the
TapFrameBuffer type.
*/
class FrameBufferCache
{
  
};

/*
A TapFrame encapsulates a TapFrameBuffer and defines the control and access
semantics for that type. A single TapFrame can own and manage multiple TFBs
over its life time. It provides easy access to commonly used offsets and
indexing capabilty throught bytes [0..Capacity).
There are no copy semantics but move semantics are provided to support single
ownership of the TBF.
*/
class  TapFrame
{
  friend class TapFrameProperties;
public:
  explicit TapFrame(bool alloc_tfb = false) :
    payload_len_(0),
    tfb_(nullptr)
  {
    if(alloc_tfb)
      tfb_ = new TapFrameBuffer;
  }

  TapFrame(const TapFrame & frame) = delete;

  TapFrame(TapFrame && frame)
  {
    tfb_ = frame.tfb_;
    frame.tfb_ = nullptr;
    payload_len_ = frame.payload_len_;
  }

  /*
  Copies the specifed amount of data into the TFB.
  */
  TapFrame(unsigned char * data, size_t len) :
//    payload_len_(len > TincanParameters::kBufferSize ?
//      TincanParameters::kBufferSize : len)
  payload_len_(100)
  {
    if(!tfb_) {
      tfb_ = new TapFrameBuffer;
    }
    memcpy(tfb_->payload, data, payload_len_);
  }

  ~TapFrame()
  {
    delete tfb_;
  }

  TapFrame & operator= (const TapFrame & rhs) = delete;

/*
Moves the TFB from the source frame to the destination
*/
  TapFrame & operator= (TapFrame && rhs)
  {
    this->tfb_ = rhs.tfb_;
    rhs.tfb_ = nullptr;
    this->payload_len_ = rhs.payload_len_;
    rhs.payload_len_ = 0;
    return *this;
  }

/*
Compares based on the pointer address of the TFB
*/
  bool operator==(const TapFrame & rhs) const
  {
    return tfb_ == rhs.tfb_;
  }

  bool operator!=(const TapFrame & rhs) const
  {
    return !(*this == rhs);
  }

  bool operator !() const
  {
    return !tfb_;
  }
/*
Used to byte address into the payload
*/
  const uint8_t & operator[](size_t index) const
  {
    if(!tfb_ || index >= sizeof(tfb_->payload))
      throw TCEXCEPT("Index out of bounds");
    return reinterpret_cast<uint8_t*>(tfb_->payload)[index];
  }

  //TapFrame & Append(const char * data, size_t len)
  //{}

  /*
  Sets the TFB source UID
  */
  void SourceUid(const string & src_uid)
  {
    if(!tfb_) {
      tfb_ = new TapFrameBuffer;
    }
    size_t len = (src_uid.length() > TincanParameters::kUidLen) ?
      TincanParameters::kUidLen : src_uid.length();
    memcpy(tfb_->payload, src_uid.c_str(), len);
  }

  /*
  Sets the TFB UID
  */
  void DestinationUid(const string & dest_uid)
  {
    if(!tfb_) {
      tfb_ = new TapFrameBuffer;
    }
//    size_t len = (dest_uid.length() > TincanParameters::kUidLen) ?
//      TincanParameters::kUidLen : dest_uid.length();
    size_t len = 100;
    memcpy(tfb_->payload, dest_uid.c_str(), len);
  }

  void Payload(uint8_t * data, size_t len)
  {
    if(!tfb_) {
      tfb_ = new TapFrameBuffer;
    }
//    payload_len_ = len > TincanParameters::kBufferSize ?
//      TincanParameters::kBufferSize : len;
    payload_len_ = 100;
    memcpy(tfb_->payload, data, payload_len_);
  }

  string && SourceUid()
  {
    if(!tfb_)
     return string();
    return string((const char*)tfb_->payload, TincanParameters::kUidLen);
  }

  string && DestinationUid()
  {
    if (!tfb_)
      return string();
    return string((const char*)tfb_->payload, TincanParameters::kUidLen);
  }

  uint8_t * Payload()
  {
    if(!tfb_)
      return nullptr;
    return tfb_->payload;
  }

  size_t PayloadLength()
  {
    return payload_len_;
  }

  /*
  Capacity is the maximum payload size
  */
  size_t Capacity()
  {
    if(!tfb_)
      return 0;
    return sizeof(tfb_->payload);
  }

  void SwapUids()
  {
    if (tfb_)
      std::swap(tfb_->src_id, tfb_->trg_id);
  }
  
  bool IsBroadcast() const {
    for (int i = 0; i < 6; i++) {
      if (tfb_->payload[i] != 0xff) { return false; }
    }
    return true;
  }

private:
  TapFrame & Initialize() {
    if(!tfb_) {
      tfb_ = new TapFrameBuffer;
    }
    memset(tfb_, 0x0, sizeof(TapFrameBuffer));
    payload_len_ = 0;
    return *this;
  }
  
  size_t payload_len_; // length of payload in bytes
  TapFrameBuffer * tfb_;
};

/*
TapFrameProperties is a read only accessor class for querying compound
properties of the TFB.
*/
class TapFrameProperties{
public:
  TapFrameProperties(const TapFrame & tf);
//  TapFrameProperties(TapFrame & tf);
  
  bool IsIccMsg() const;
  bool IsIp4() const;
  bool IsIp6() const;
  bool IsArpRequest() const;
  bool IsArpResponse() const;
  bool IsUnicast() const;
  bool IsBroadcast() const;
//  TapFrameProperties(const TapFrame & tf) :
//    tf_(tf)
//  {}
//
//  bool IsIccMsg() const
//  {
//    return false;
//  }
//
//  bool IsIp4() const
//  {
//    return false;
//  }
//
//  bool IsIp6() const
//  {
//    return false;
//  }
//
//  bool IsArpRequest() const
//  {
//    return false;
//  }
//
//  bool IsArpResponse() const
//  {
//    return false;
//  }
//
//  bool IsUnicast() const
//  {
//    return false;
//  }
//
//  bool IsBroadcast() const
//  {
//    return false;
//  }

private:
  const TapFrame & tf_;
//  TapFrame & tf_;
};

}
#endif  // TINCAN_TAPFRAME_H_
