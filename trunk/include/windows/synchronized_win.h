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
                          WinService
                      -------------------
    description			: Win32 Service API wrapper class.
  Wrappers for a few useful synchronization primitives. Simply instantiate
  a Synchronized obejct with a Mutex or CriticalSection. All the code in the
  scope of that object will be serialized on your Lock.
    1. Lock - Abstract base class
    2. Mutex - Mutex implementation of Lock
    3. CriticalSection - CriticalSection implelmentation of Lock
    4. Synchronized - Uses 2 or 3 to serialize a block of code
***************************************************************************/

#if !defined(_TINCAN_SYNCHRONIZED_H_)
#define _TINCAN_SYNCHRONIZED_H_

#include "lock.h"

#if defined(_IPOP_WIN)
#include <Windows.h>

namespace tincan
{
namespace windows
{
class MutexWin :
  virtual public Lock
{
public:
  MutexWin();

  ~MutexWin();

  void lock();

  void unlock();

private:
  HANDLE mutex;
};

class CriticalSectionWin :
  virtual public Lock
{
public:
  CriticalSectionWin();

  ~CriticalSectionWin();

  void lock();

  void unlock();

private:
  CRITICAL_SECTION cs;
};

class SynchronizedWin
{
public:
  SynchronizedWin(Lock & object);

  ~SynchronizedWin();

protected:
  Lock & mObject;
};
} // namespace windows
} // namespace tincan
#endif // _IPOP_WIN
#endif // _TINCAN_SYNCHRONIZED_H_
