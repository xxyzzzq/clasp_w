/* The criterion_exp class used for evaluating the demand rate to stop the solver by checking
   how much the current value approximates the expected value

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

#ifndef CRITERION_EXP_H__
#define CRITERION_EXP_H__

#include "criterion.h"

namespace Clasp_W {
class Criterion_Exp : public Criterion {
public:
  Criterion_Exp(int64 exp_val);
  Criterion_Exp(const Criterion_Exp &right);
  virtual ~Criterion_Exp(void);
  const Criterion_Exp &operator = (const Criterion_Exp &right);

public:
  virtual void clear(void);
  virtual double check(uint64 time, int64 value);
  virtual Criterion *clone(void) const;

private:
  int64 exp_val_;
};
}  // namespace Clasp_W


#endif  // CRITERION_EXP_H__
