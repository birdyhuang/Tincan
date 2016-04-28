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
#include <cstddef>

using namespace tincan;
const char TincanParameters::kContentName[] = "ipop-";
const char TincanParameters::kIceUfrag[] = "ufrag";
const char TincanParameters::kIcePwd[] = "pwd";
const char TincanParameters::kLocalHost[] = "127.0.0.1";
const char TincanParameters::kLocalHost6[] = "::1";
const char TincanParameters::kIpv4[] = "172.31.0.100";
const char TincanParameters::kIpv6[] = "fd50:0dbc:41f2:4a3c:0000:0000:0000:0000";
const int TincanParameters::kDefaultXmppPort = 5222;
const int TincanParameters::kBufferSize = 1024;
//const int TincanParameters::kBufferSize = 1500;
const char TincanParameters::kIpopVer = 0x03;
const char TincanParameters::kTincanControl = 0x01;
const char TincanParameters::kTincanPacket = 0x02;
const char TincanParameters::kICCControl = 0x03;
const char TincanParameters::kICCPacket = 0x04;
const char TincanParameters::kFprNull[] =
"00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";
const uint32_t TincanParameters::kLocalControllerId = 0;
const size_t TincanParameters::kIdBytesLen = 20;
const size_t TincanParameters::kFlags = 0;
unsigned short TincanParameters::kUdpPort = 5800;

int main(int argc, char **argv)
{
  int rv = 0;
  try {
    Tincan tc;
    tc.Start();
    tc.Shutdown();
  }
  catch(exception & e) {
    rv = -1;
    LOG_F(LS_ERROR) << e.what();
  }

  return rv;

}