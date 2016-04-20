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
#ifndef TINCAN_PARAMETERS_H_
#define TINCAN_PARAMETERS_H_
namespace tincan
{
class TincanParameters
{
public:
  //
  static int kUdpPort;

  //
  static const char kLocalHost[];

  //
  static const char kLocalHost6[];

  //
  static const int kDefaultXmppPort;

  //
  static const int kBufferSize;

  //
  static const char kIpopVer;

  //
  static const char kTincanControl;

  //
  static const char kTincanPacket;

  //Intercontroller connection header
  static const char kICCControl;

  //Intercontroller connection header
  static const char kICCPacket = 0x04;
};

int kUdpPort = 5800;
const char TincanParameters::kLocalHost[] = "127.0.0.1";
const char TincanParameters::kLocalHost6[] = "::1";
const int TincanParameters::kDefaultXmppPort = 5222;
const int TincanParameters::kBufferSize = 1024;
const char TincanParameters::kIpopVer = 0x03;
const char TincanParameters::kTincanControl = 0x01;
const char TincanParameters::kTincanPacket = 0x02;
const char TincanParameters::kICCControl = 0x03;
static const char kICCPacket = 0x04;

} // namespace tincan
#endif // TINCAN_PARAMETERS_H_
