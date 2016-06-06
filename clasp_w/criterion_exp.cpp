/* The criterion_exp class used for evaluating the demand rate to stop the solver by checking
   the approximation of the current value to the expected value

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

#include "criterion_exp.h"

using namespace Clasp_W;

Criterion_Exp::Criterion_Exp(int64 exp_val)
  : Criterion() {
  this->exp_val_ = exp_val;
}

Criterion_Exp::Criterion_Exp(const Clasp_W::Criterion_Exp &right)
  : Criterion(right) {
  this->exp_val_ = right.exp_val_;
}

const Criterion_Exp &Criterion_Exp::operator =(const Clasp_W::Criterion_Exp &right) {
  Criterion::operator =(right);
  this->exp_val_ = right.exp_val_;
  return *this;
}

Criterion_Exp::~Criterion_Exp(void) {
}

void Criterion_Exp::clear(void) {
}

double Criterion_Exp::check(uint64 time, int64 value) {
  if (value < 0) {
    value = 0;
  }
  return (double)value / this->exp_val_;
}

Criterion *Criterion_Exp::clone(void) const {
  return new Criterion_Exp(*this);
}
