/* Wrapper class for the Original Clasp solver providing Clasp_W interface

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

#ifndef CLASP_SOLVER_H_
#define CLASP_SOLVER_H_

#include <vector>
#include <iostream>
#include <pthread.h>
#include <memory>
#include <clasp/clasp_facade.h>
#include "clock.h"
#include "stopper.h"

namespace Clasp_W {
class Clasp_Solver {
public:
  Clasp_Solver(void);
  virtual ~Clasp_Solver(void);

private:
  Clasp_Solver(const Clasp_Solver& from);
  const Clasp_Solver& operator = (const Clasp_Solver& right);

public:
  virtual void solve_timeout(void);
  virtual void solve(void);

  const std::vector<bool> &getModel(void) const;
  const Clasp_W::Clock &getClock(void) const;
  int64 getBestGoalValue(void) const;
  bool isSolutionFound(void) const;
  bool isFinished(void) const;

  void setStopper(const std::shared_ptr<Stopper> &stopper );
  const std::shared_ptr<Stopper> &getStopper(void) const {
    return stopper_;
  }
  void setStartValue(int start_value);

  void setSeed(unsigned int srand);
  unsigned int getSeed(void) const;
  void setIsRand(bool is_rand);
  void setIsDebug(bool is_debug);

  void setInput(std::istream *ins);

private:
  void resetStats(void);
  void setJumpy(void);

private:
  std::shared_ptr<Clasp::ClaspConfig> claspconfig_;
  unsigned int srand_;
  bool is_rand_;
  bool is_debug_;

  bool is_started_;
  bool is_solution_found_;
  bool is_finished_;
  int64 best_goal_value_;
  int64 start_value_;
  std::vector<bool> best_model_;
  Clasp_W::Clock clock_;  // only use with solve with time out
  std::shared_ptr<Stopper> stopper_;
  pthread_mutex_t stats_lock_;

  std::istream *ins_;

  friend class StoreModelCallback;
  friend void *solver_proc(void *);
};
}  // namespace Clasp_W

#endif  // CLASP_SOLVER_H_
