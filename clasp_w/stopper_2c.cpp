/*
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

#include <iostream>
#include "stopper_2c.h"

using namespace Clasp_W;

Stopper_2C::Stopper_2C(const boost::shared_ptr<Criterion> &primary_criterion,
                                     const boost::shared_ptr<Criterion> &secondary_criterion,
                                     double k,
                                     double threshold)
    : Stopper() {
  this->primary_criterion_ = primary_criterion;
  this->secondary_criterion_ = secondary_criterion;
  this->threshold_ = threshold;
  this->k_ = k;
}

Stopper_2C::Stopper_2C(const Clasp_W::Stopper_2C &from)
    : Stopper(from) {
  this->primary_criterion_.reset(from.primary_criterion_->clone());
  this->secondary_criterion_.reset(from.secondary_criterion_->clone());
  this->threshold_ = from.threshold_;
  this->k_ = from.k_;
}

Stopper_2C::~Stopper_2C(void) {
}

const Stopper_2C &Stopper_2C::operator = (const Stopper_2C & right) {
  Stopper::operator =(right);
  this->primary_criterion_.reset(right.primary_criterion_->clone());
  this->secondary_criterion_.reset(right.secondary_criterion_->clone());
  this->threshold_ = right.threshold_;
  this->k_ = right.k_;
  return *this;
}

void Stopper_2C::clear(void) {
  this->primary_criterion_->clear();
  this->secondary_criterion_->clear();
}

Stopper *Stopper_2C::clone(void) const {
  return new Stopper_2C(*this);
}

bool Stopper_2C::check(uint64 time, int64 value) {
  if (time <= 0) {
    time = 1;
  }
  if (value < 0) {
    value = 0;
  }
  double c_1 = this->primary_criterion_->check(time, value);
  double c_2 = this->secondary_criterion_->check(time, value);
  double c = this->evaluate_func(c_1, c_2);
  if (this->isDebug()) {
    static int count = 0;
    count = (count+1) % 100;
    if (count == 0) {
      std::cerr << "time=";
      std::cerr.width(5);
      std::cerr << time << " value=";
      std::cerr.width(5);
      std::cerr << value;
      std::cerr.width(8);
      std::cerr << " c1=";
      std::cerr.width(8);
      std::cerr << c_1 << " c2=";
      std::cerr.width(8);
      std::cerr << c_2 << " c=";
      std::cerr.width(8);
      std::cerr << c << std::endl;
    }
  }
  return c - this->threshold_ >= -1e-6;
}

double Stopper_2C::evaluate_func(double c_1, double c_2) const {
  return c_1 + this->k_ * c_1 * c_2;
}

void Stopper_2C::setIsDebug(bool is_debug) {
  Stopper::setIsDebug(is_debug);
  this->primary_criterion_->setIsDebug(is_debug);
  this->secondary_criterion_->setIsDebug(is_debug);  
}
