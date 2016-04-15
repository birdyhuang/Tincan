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
#include "tincan_control.h"

namespace tincan
{
TincanControl::TincanControl(
  const char * const data,
  const size_t len) :
  data_(data),
  data_len_(len)
{
  //todo: kIpopVer
  int kIpopVer=0;
  if(data[INDEX_VERSION] != kIpopVer) {
    string msg = "Invalid version stamp (byte zero) in control message:";
    msg.append(data);
    throw exception(msg.c_str());
  }
  message_.assign(data_, 2, data_len_);
}

TincanControl::~TincanControl()
{

}
string TincanControl::Name() const
{
  return string();
}

const unsigned char TincanControl::Type() const
{
  return data_[INDEX_TYPE];
}

void
TincanControl::AsJson(Json::Value & dict)
{
  Json::Reader reader;

  if(!reader.parse(message_, dict)) {
    string errmsg = "{\"error\":\"json parsing failed\"}";
    throw exception(errmsg.c_str());
  }
  return;
}
} // namespace tincan
