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
/**************************************************************************
  Wrappers for a few useful synchronization primitives. Simply instantiate
  a Synchronized obejct with a Mutex or CriticalSection. All the code in the
  scope of that object will be serialized on your Lock.
    1. Lock - Abstract base class
    2. Mutex - Mutex implementation of Lock
    3. CriticalSection - CriticalSection implelmentation of Lock
    4. Synchronized - Uses 2 or 3 to serialize a block of code
***************************************************************************/

#if !defined (_TINCAN_SYNCHRONIZED_H_)
#define _TINCAN_SYNCHRONIZED_H_

#include "lock.h"
#if defined(_IPOP_LINUX)
#include "linux/synchronized_lnx.h"
#elif defined(_IPOP_OSX)
#include "mac/synchronized_mac.h"
#elif defined(_IPOP_WIN)
#include "windows/synchronized_win.h"
#endif

namespace tincan
{
using namespace windows;
class Mutex :
#if defined(_IPOP_WIN)
  virtual public MutexWin
#elif defined(_IPOP_LINUX)
  virtual public MutexLnx
#elif defined(_IPOP_OSX)
  virtual public MutexOsx
#endif
{};

class CriticalSection :
#if defined(_IPOP_WIN)
  virtual public CriticalSectionWin
#elif defined(_IPOP_LINUX)
  virtual public CriticalSectionLnx
#elif defined(_IPOP_OSX)
  virtual public CriticalSectionOsx
#endif
{};

class Synchronized :
#if defined(_IPOP_WIN)
  public SynchronizedWin
#elif defined(_IPOP_LINUX)
  public SynchronizedLnx
#elif defined(_IPOP_OSX)
  public SynchronizedOsx
#endif

{
public:
#if defined(_IPOP_WIN)
  Synchronized(Lock & lock) :
    SynchronizedWin(lock)
  {}
#elif defined(_IPOP_LINUX)
  Synchronized(Lock & lock) :
    SynchronizedLnx(lock)
  {}
#elif defined(_IPOP_OSX)
  Synchronized(Lock & lock) :
    SynchronizedOsx(lock)
  {}
#endif
}; // class Synchronized
} // namespace tincan
#endif // _TINCAN_SYNCHRONIZED_H_
