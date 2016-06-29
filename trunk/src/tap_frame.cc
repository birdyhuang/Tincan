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

#include "tap_frame.h"
#include <iostream>

using namespace std;

namespace tincan {

TapFrameProperties::TapFrameProperties(const TapFrame & tf) :
  tf_(tf)
{}

bool TapFrameProperties::IsIccMsg() const
{
  /** TODO: add some comments */
  return tf_[0] == 0x00 && tf_[1] == 0x69 && tf_[2] == 0x70 && tf_[3] == 0x6f && tf_[4] == 0x70;
}

bool TapFrameProperties::IsIp4() const
{
  /** TODO: why start from 14? */
  return (tf_[14] >> 4) == 0x04;
}

bool TapFrameProperties::IsIp6() const
{
  /** TODO: same as above */
  return (tf_[14] >> 4) == 0x06;
}

bool TapFrameProperties::IsArpRequest() const
{
  /* 
   * 12-13: ether_type (0x0806 for arp)
   * 21: arp_operation (1:arp request, 2:arp response, 3:rarp request, 4:rarp response) 
   */
  return tf_[12] == 0x08 && tf_[13] == 0x06
      && tf_[21] == 0x01;
}

bool TapFrameProperties::IsArpResponse() const
{
  /*
   * 12-13: ether_type (0x0806 for arp)
   * 21: arp_operation (1:arp request, 2:arp response, 3:rarp request, 4:rarp response)
   */
  return tf_[12] == 0x08 && tf_[13] == 0x06
      && tf_[21] == 0x02;
}

bool TapFrameProperties::IsUnicast() const
{
  return false;
}

bool TapFrameProperties::IsBroadcast() const
{
  return tf_.IsBroadcast();
}

}