/* The criterion class used for evaluating the demand rate to stop the solver

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

#ifndef CRITERION_H__
#define CRITERION_H__

#include <clasp/util/platform.h>

namespace Clasp_W {
class Criterion {
public:
  Criterion(void);
  Criterion(const Criterion &right);
  virtual ~Criterion(void) = 0;
  const Criterion &operator = (const Criterion &right);

public:
  virtual void clear(void) = 0;
  virtual double check(uint64 time, int64 value);
  virtual Criterion *clone(void) const = 0;
  void setIsDebug(bool is_debug);
  bool isDebug(void) const;

private:
  bool is_debug_;
};
}  // namespace Clasp_W


#endif  // CRITERION_H__
