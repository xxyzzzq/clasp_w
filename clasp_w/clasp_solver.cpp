/* Definition of the wrapper class for the Clasp solver

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

#include "clasp_solver.h"
#include <clasp/clasp_facade.h>
#include <clasp/satelite.h>
#include <clasp/minimize_constraint.h>
#include <iostream>
#include <memory>
#include <pthread.h>
#include "stopper_window.h"

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#define usleep(x) Sleep(x/1000)
#endif  // _MSC_VER

using namespace Clasp_W;

namespace Clasp_W {
class StoreModelCallback : public Clasp::ClaspFacade::Callback {
public:
  typedef Clasp::ClaspFacade::Event Event;

  StoreModelCallback(Clasp_Solver *stats) {
    this->stats_ = stats;
  }
  virtual ~StoreModelCallback() {}

	void warning(const char* msg) { std::cerr << "Warning: " << msg << std::endl; }
	void state(Event, Clasp::ClaspFacade&) {}

	void event(const Clasp::Solver& s, Event e, Clasp::ClaspFacade& f) {
	  if (e == Clasp::ClaspFacade::event_p_prepared) {
	    pthread_mutex_lock(&this->stats_->stats_lock_);
	    std::cout << "c started" << std::endl;
	    this->stats_->is_started_ = true;
	    this->stats_->clock_.reset();
	    pthread_mutex_unlock(&this->stats_->stats_lock_);
	  }
		if (e == Clasp::ClaspFacade::event_model) {
	    pthread_mutex_lock(&this->stats_->stats_lock_);
		  const Clasp::Assignment &a = s.assignment();
		  this->stats_->best_model_.resize(a.numVars()-1);
		  // start from 1
      for (std::size_t i = 0; i < a.numVars()-1; ++i) {
        if (a.value(i+1) == Clasp::value_true) {
          this->stats_->best_model_[i] = true;
        } else {
          this->stats_->best_model_[i] = false;
        }
      }
      // TODO: update best goal value!!!
      if (s.sharedContext()->enumerator()->minimize() != 0) {
        this->stats_->best_goal_value_ = s.sharedContext()->enumerator()->minimize()->optimum()->opt[0];
        std::cout << "o " << this->stats_->best_goal_value_ << std::endl;
        //std::cerr << this->stats_->clock_.getTime() << "\t" << this->stats_->best_goal_value_ << std::endl;
      }
      if (this->stats_->best_goal_value_ < this->stats_->start_value_) {
        this->stats_->is_solution_found_ = true;
      }
      {
        // check immediately here
        Clasp_Solver *solver = this->stats_;
        if (solver->stopper_.get()) {
          if (solver->stopper_->check(solver->clock_.getTime(), solver->start_value_-solver->best_goal_value_)) {
	          pthread_mutex_unlock(&this->stats_->stats_lock_);
            pthread_exit(0);
          }
        }
      }
	    pthread_mutex_unlock(&this->stats_->stats_lock_);
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
      pthread_testcancel();
      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
   	}
  }
  
private:
  Clasp_Solver *stats_;
};
}  // namespace Clasp_W

Clasp_Solver::Clasp_Solver(void) {
  this->srand_ = 0;
  this->start_value_ = 0;
  this->is_debug_ = false;
  pthread_mutex_init(&this->stats_lock_, 0);
  this->resetStats();
}

Clasp_Solver::~Clasp_Solver(void) {
}
  
Clasp_Solver::Clasp_Solver(const Clasp_Solver& from) {
  this->srand_ = from.srand_;
  this->best_goal_value_ = from.best_goal_value_;
  this->best_model_ = from.best_model_;
  this->is_solution_found_ = from.is_solution_found_;
  this->is_started_ = from.is_started_;
  this->is_debug_ = from.is_debug_;
  this->is_finished_ = from.is_finished_;
  this->clock_ = from.clock_;
  this->stopper_.reset(this->stopper_->clone());
  this->start_value_ = from.start_value_;
  this->stats_lock_ = from.stats_lock_;
}

const Clasp_Solver& Clasp_Solver::operator = (const Clasp_Solver& right) {
  this->srand_ = right.srand_;
  this->best_goal_value_ = right.best_goal_value_;
  this->best_model_ = right.best_model_;
  this->is_solution_found_ = right.is_solution_found_;
  this->is_started_ = right.is_started_;
  this->is_debug_ = right.is_debug_;
  this->is_finished_ = right.is_finished_;
  this->clock_ = right.clock_;
  this->stopper_.reset(this->stopper_->clone());
  this->start_value_ = right.start_value_;
  this->stats_lock_ = right.stats_lock_;
  return *this;
}

namespace Clasp_W {
void *solver_proc(void *args) {
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  Clasp_Solver *solver = (Clasp_Solver *)args;
  solver->solve();
  solver->is_finished_ = true;
  return 0;
}
}

void Clasp_Solver::solve_timeout(void) {
  pthread_t tid;
  this->clock_.reset();
  pthread_create(&tid, 0, solver_proc, this);
  if (this->stopper_.get()){
    while (true) {
      pthread_mutex_lock(&this->stats_lock_);
      if (this->is_started_ &&
          this->stopper_->check(this->clock_.getTime(), this->start_value_-this->best_goal_value_)) {
        pthread_mutex_unlock(&this->stats_lock_);
        break;
      }
      if (this->isFinished()) {
        pthread_mutex_unlock(&this->stats_lock_);
        break;
      }
      pthread_mutex_unlock(&this->stats_lock_);
      usleep(1000);
    }
  }
  pthread_cancel(tid);
  pthread_join(tid, 0);
}

void Clasp_Solver::solve(void) {
  this->resetStats();
  Clasp::ClaspFacade libclasp;
  StoreModelCallback cb(this);
  this->claspconfig_.reset(new Clasp::ClaspConfig());
  this->setJumpy();
  this->claspconfig_->master()->solver->strategies().rng.srand(this->srand_);
  if (this->is_rand_) {
    this->claspconfig_->master()->params.setRandomProbability(1.0);
  }
  Clasp::StreamInput input(*this->ins_, Clasp::Input::OPB);
  libclasp.solve(input, *this->claspconfig_, &cb);
}

const std::vector<bool> &Clasp_Solver::getModel(void) const {
  return this->best_model_;
}

const Clasp_W::Clock &Clasp_Solver::getClock(void) const {
  return this->clock_;
}

int64 Clasp_Solver::getBestGoalValue(void) const {
  return this->best_goal_value_;
}

bool Clasp_Solver::isSolutionFound(void) const {
  return this->is_solution_found_;
}

bool Clasp_Solver::isFinished(void) const {
  return this->is_finished_;
}
  
void Clasp_Solver::setStopper(const boost::shared_ptr<Stopper> &stopper) {
  this->stopper_ = stopper;
}

void Clasp_Solver::setStartValue(int start_value) {
  this->start_value_ = start_value;
}

void Clasp_Solver::setSeed(unsigned int srand) {
  this->srand_ = srand;
}

unsigned int Clasp_Solver::getSeed(void) const {
  return this->claspconfig_->master()->solver->strategies().rng.seed();
}

void Clasp_Solver::setIsRand(bool is_rand) {
  this->is_rand_ = is_rand;
}

void Clasp_Solver::setIsDebug(bool is_debug) {
  this->is_debug_ = is_debug;
  if (this->is_debug_ && this->stopper_.get()) {
    this->stopper_->setIsDebug(this->is_debug_);
  }
}

void Clasp_Solver::setInput(std::istream *ins) {
  this->ins_ = ins;
}

void Clasp_Solver::resetStats(void) {
  this->is_started_ = false;
  this->is_solution_found_ = false;
  this->is_finished_ = false;
  this->best_goal_value_ = std::numeric_limits<int>::max();
  this->best_model_.clear();
  if (this->stopper_.get()) {
    this->stopper_->clear();
  }
}

void Clasp_Solver::setJumpy(void) {
  // --sat-p=20,25,240,-1,1
  std::auto_ptr<Clasp::SatElite::SatElite> pre(new Clasp::SatElite::SatElite());
  pre->options.maxIters = 20;
  pre->options.maxOcc = 25;
  pre->options.maxTime = 240;
  this->claspconfig_->ctx.satPrepro.reset(pre.release());
  // --trans-ext=dynamic
  this->claspconfig_->eq.erMode = Clasp::ProgramBuilder::mode_transform_dynamic;
  // --heuristic=Vsids
  this->claspconfig_->master()->solver->strategies().heuId = 2;
  // --restarts=L,100
  {
	  Clasp::RestartParams &p = this->claspconfig_->master()->params.restart;
    p.sched.base = 100;
    p.sched.type = 2;
    p.sched.idx = 0;
    p.sched.len = 0;
    p.sched.grow = 0.0;
  }
  {
    // --del-init-r=1000,20000
    Clasp::ReduceParams &p = this->claspconfig_->master()->params.reduce;
    p.initRange.lo = 1000;
    p.initRange.hi = 20000;
    // --del-algo=basic,2
    p.strategy.score = 2;
    // --deletion=3,75
    p.strategy.fReduce = 75;
    // --del-grow=1.1,25,x,100,1.5
    p.fGrow = 1.1f;
    p.fMax = 25;
    p.growSched.base = 100;
    p.growSched.type = 0;
    p.growSched.grow = 1.5;
    // --del-cfl=x,10000,1.1
    p.cflSched.base = 10000;
    p.cflSched.grow = 1.1f;
    // --del-glue=2
    p.strategy.glue = 2;
  }
  // --update-lbd=3
  this->claspconfig_->master()->solver->strategies().updateLbd = 3;
  // --strengthen=recursive
  this->claspconfig_->master()->solver->strategies().strRecursive = 1;
  // --otfs=2
  this->claspconfig_->master()->solver->strategies().otfs = 2;
  // --save-p=70
  this->claspconfig_->master()->solver->strategies().saveProgress = 70;
}
