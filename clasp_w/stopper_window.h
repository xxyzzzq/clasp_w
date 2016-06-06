/* A stopper class using a window

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

#ifndef STOPPER_WINDOW_H_
#define STOPPER_WINDOW_H_
#include <deque>
#include <clasp/util/platform.h>
#include "stopper.h"
#include "criterion_window.h"

namespace Clasp_W {
class Stopper_Window : public Stopper {
public:
  Stopper_Window(
      uint64 min_time_out,
      double window_ratio_y,
      double threshold);
  Stopper_Window(const Stopper_Window &right);
  virtual ~Stopper_Window(void);
  const Stopper_Window &operator = (const Stopper_Window &right);

public:
  virtual void clear(void);
  virtual bool check(uint64 time, int64 value);
  virtual Stopper *clone(void) const;
  virtual void setIsDebug(bool is_debug);

private:
  Criterion_Window criterion_w_;
  double threshold_;
};
}  // namespace Clasp_W

#endif  // STOPPER_WINDOW_H_
