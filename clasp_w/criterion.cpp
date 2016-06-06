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

#include "criterion.h"

using namespace Clasp_W;

Criterion::Criterion(void) {
  this->is_debug_ = false;
}

Criterion::Criterion(const Clasp_W::Criterion &from) {
  this->is_debug_ = from.is_debug_;
}

Criterion::~Criterion(void) {
}

const Criterion &Criterion::operator = (const Criterion &right) {
  this->is_debug_ = right.is_debug_;
  return (*this);
}

double Criterion::check(uint64 time, int64 value) {
  return 0.0;
}

void Criterion::setIsDebug(bool is_debug) {
  this->is_debug_ = is_debug;
}

bool Criterion::isDebug(void) const {
  return this->is_debug_;
}
