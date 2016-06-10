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
  void ParseCmdlineArgs(
    int argc,
    char **args)
  {
    if(argc == 2 && strncmp(args[1], "-v", 2) == 0) {
      kVersionCheck = true;
    }
    else if(argc == 2 && strncmp(args[1], "-h", 2) == 0) {
//      kNeedsHelp;
    }
    if(argc == 2 && strncmp(args[1], "-p=", 3) == 0) {
      TincanParameters::kUdpPort = (unsigned short)atoi(args[1]+3);
    }
  }
  //
  static const unsigned short kTincanVerMjr;

  static const unsigned short kTincanVerMnr;

  static const unsigned short kTincanVerRev;

  //
  static const char kIceUfrag[];

  //
  static const char kIcePwd[];

  //
  static unsigned short kUdpPort;

  //
  static const char kLocalHost[];

  //
  static const char kIpv4[];

  //
  static const char kLocalHost6[];

  //
  static const char kIpv6[];

  //
  static const unsigned short kDefaultXmppPort;

  //
  static const unsigned short kBufferSize;

  static const unsigned short kFrameHeaderSize;

  //
  static const char kIpopVer;

  //
  static const char kTincanControl;

  //
  static const char kTincanPacket;

  //Intercontroller connection header
  static const char kICCControl;

  //Intercontroller connection header
  static const char kICCPacket;

  //
  static const size_t kFlags;

  //
  static const char kFprNull[];

  //
  static const uint32_t kLocalControllerId;

  //
  static const unsigned short kUidLen;

  static bool kVersionCheck;

  static bool kNeedsHelp;

};
} // namespace tincan
#endif // TINCAN_PARAMETERS_H_
