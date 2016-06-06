/* A stopwatch-like utility class

Copyright (c) 2012-2014 Zhiqiang Zhang, Institute of Software, Chinese Academy of Sciences

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifdef _MSC_VER
#include <windows.h>
#else  // _MSC_VER
#include <sys/times.h>
#include <unistd.h>
#endif  // _MSC_VER

#include "clock.h"

using namespace Clasp_W;

Clock::Clock(void) {
  this->start_ = 0;
}

Clock::Clock(const Clock &from) {
  this->start_ = from.start_;
}

Clock::~Clock(void) {
}

const Clock &Clock::operator = (const Clock &right) {
  this->start_ = right.start_;
  return *this;
}

void Clock::reset(void) {
  this->start_ = getUserTime();
}
  
uint64 Clock::getTime(void) const {
  return getUserTime() - this->start_;
}

uint64 Clock::getUserTime(void) {
#ifdef _MSC_VER
  FILETIME ft1, ft2, ft3, ft4;
  GetProcessTimes((HANDLE)-1, &ft1, &ft2, &ft3, &ft4);
  uint64 val = ((uint64)ft4.dwHighDateTime << 32) + ft4.dwLowDateTime;
  return val/10000;
#else  // _MSC_VER
  tms t;
  times(&t);
  uint64 tck = sysconf(_SC_CLK_TCK);
  return t.tms_utime*1000/tck;
#endif  // _MSC_VER
}
