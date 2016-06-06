/* Definition of the window stopper class

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

#include <cassert>
#include <iostream>
#include "stopper_window.h"

using namespace Clasp_W;

Stopper_Window::Stopper_Window(
    uint64 min_time_out,
    double window_ratio_y,
    double threshold)
    : Stopper(),
    criterion_w_(min_time_out, window_ratio_y) {
  this->threshold_ = threshold;
}

Stopper_Window::Stopper_Window(const Stopper_Window &from)
    : Stopper(from), criterion_w_(from.criterion_w_) {
  this->threshold_ = from.threshold_;
}

Stopper_Window::~Stopper_Window(void) {
}

const Stopper_Window &Stopper_Window::operator =(const Stopper_Window &right) {
  Stopper::operator =(right);
  this->criterion_w_ = right.criterion_w_;
  this->threshold_ = right.threshold_;
  return *this;
}

void Stopper_Window::clear(void) {
  this->criterion_w_.clear();
}

bool Stopper_Window::check(uint64 time, int64 value) {
  double c = this->criterion_w_.check(time, value);
  //std::cerr << "time=" << time << " value=" << value << " c=" << c << std::endl;
  return this->criterion_w_.check(time, value) - this->threshold_ >= -1e-6;
}

Stopper *Stopper_Window::clone(void) const {
  return new Stopper_Window(*this);
}

void Stopper_Window::setIsDebug(bool is_debug) {
  Stopper::setIsDebug(is_debug);
  this->criterion_w_.setIsDebug(is_debug);
}
