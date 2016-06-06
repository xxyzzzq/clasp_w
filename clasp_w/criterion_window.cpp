/* The criterion_window class used for evaluating the demand rate to stop the solver by checking
   how slow the solve to yeild better solutions

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
#include "criterion_window.h"

using namespace Clasp_W;

Criterion_Window::Criterion_Window(uint64 min_time_out, double ratio_y)
    : Criterion() {
  this->min_time_out_ = min_time_out;
  this->ratio_y_ = ratio_y;
  this->history_records_.push_back(std::pair<uint64, int64>(0, 0));
}

Criterion_Window::Criterion_Window(const Criterion_Window &from)
    : Criterion(from) {
  this->min_time_out_ = from.min_time_out_;
  this->ratio_y_ = from.ratio_y_;
  this->history_records_ = from.history_records_;
}

const Criterion_Window &Criterion_Window::operator =(const Criterion_Window &right) {
  Criterion::operator =(right);
  this->min_time_out_ = right.min_time_out_;
  this->ratio_y_ = right.ratio_y_;
  this->history_records_ = right.history_records_;
  return *this;
}

Criterion_Window::~Criterion_Window(void) {
}

void Criterion_Window::clear(void) {
  this->history_records_.clear();
  this->history_records_.push_back(std::pair<int, int>(1, 0));
}

double Criterion_Window::check(uint64 time, int64 value) {
  if (value < 0) {
    value = 0;
  }
  if (!this->history_records_.empty() &&
    this->history_records_.back().first > time) {
    this->clear();
  }
  assert(this->history_records_.empty() ||
      this->history_records_.back().second <= value);
  if (this->history_records_.empty() ||
    this->history_records_.back().second < value) {
    this->history_records_.push_back(std::pair<uint64, uint64>(time, value));
  }
  if (time < this->min_time_out_ || value <= 0) {
    return 0;
  }
  int64 window_head_value = (int64)value * this->ratio_y_;
  while (!this->history_records_.empty()) {
    if (this->history_records_.front().second < window_head_value) {
      this->history_records_.pop_front();
    } else {
      break;
    }
  }
  uint64 head_time = this->history_records_.front().first;
  //if (head_time < this->min_time_out_) {
  //  head_time = this->min_time_out_;
  //}
  uint64 x_diff = time - head_time;
  return ((double)x_diff/head_time);
}

Criterion *Criterion_Window::clone(void) const {
  return new Criterion_Window(*this);
}
