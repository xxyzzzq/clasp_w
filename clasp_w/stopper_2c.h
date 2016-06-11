/* A stopper class using two criteria

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

#ifndef STOPPER_2C_H__
#define STOPPER_2C_H__

#include <memory>
#include <clasp/util/platform.h>
#include "stopper.h"
#include "criterion.h"

namespace Clasp_W {
class Stopper_2C : public Stopper {
public:
  Stopper_2C( const std::shared_ptr<Criterion> &primary_criterion,
              const std::shared_ptr<Criterion> &secondary_criterion,
              double k,
              double threshold);
  Stopper_2C(const Stopper_2C &from);
  virtual ~Stopper_2C(void);
  const Stopper_2C &operator = (const Stopper_2C &right);

public:
  virtual void clear(void);
  virtual bool check(uint64 time, int64 value);
  virtual Stopper *clone(void) const;
  virtual void setIsDebug(bool is_debug);

private:
  virtual double evaluate_func(double c_1, double c_2) const;
  
private:
  std::shared_ptr<Criterion> primary_criterion_;
  std::shared_ptr<Criterion> secondary_criterion_;
  double threshold_;
  double k_;
};
}  // namespace Clasp_W


#endif  // STOPPER_2C_H__