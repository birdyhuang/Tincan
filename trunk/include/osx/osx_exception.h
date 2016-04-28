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

#pragma once

#include <sstream>
#include <string>
#include <exception>
#include <cstdlib>
#include <errno.h>
using namespace std;


namespace tincan {

namespace osx {

#define OSXEXCEPT(ExtendedErrorInfo) 	OsxException(ExtendedErrorInfo, __FILE__, __LINE__)
typedef char* LPSTR;

class OsxException : public runtime_error {
    string msg;

public:
    __inline OsxException(const string &arg, const char *file, int line);
    __inline OsxException();
    __inline ~OsxException();
    __inline const char* what() const noexcept;
};

OsxException::OsxException(const string &arg, const char *file, int line) :
runtime_error(arg)
{
    ostringstream o;
    o << file << ":" << line << ": " << arg;
    msg = o.str();
}

//OsxException::OsxException() : runtime_error::runtime_error("Exception")
//{
//    OsxException("Exception", __FILE__, __LINE__);
//}

OsxException::~OsxException()
{
  //delete mSourceFile;
  //delete mHostErrorInfo;
  //delete mExtendedErrorInfo;
}

const char* OsxException::what() const noexcept
{
  return msg.c_str();
}

}
}
