/* The main entry for Clasp_W (a modified version of the Clasp solver)
   Copyright (c) 2012, Zhiqiang Zhang
   All rights reserved.

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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <clasp/clasp_facade.h>
#include <clasp/reader.h>
#include "clasp_solver.h"
#include "stopper_window.h"
#include "stopper_2c.h"
#include "criterion_window.h"
#include "criterion_exp.h"

using namespace Clasp_W;

bool process_arg_w(const char *arg, boost::shared_ptr<Stopper> &stopper_w) {
  std::string opt = arg;
  if (opt == "default") {
    opt = "2000,0.9,3.0";
  }
  if (opt.size() <= 0) {
    return false;
  }
  
  std::vector<std::string> opts;
  boost::split(opts, arg, boost::is_any_of(","));
  if (opts.size() != 3) {
    return false;
  }
  std::stringstream ss0(opts[0]);
  uint64 min_time_out;
  ss0 >> min_time_out;

  stopper_w.reset(new Stopper_Window(
      min_time_out,
      atof(opts[1].c_str()),
      atof(opts[2].c_str())));
  return true;
}

bool process_arg_c(const char *arg, boost::shared_ptr<Stopper> &stopper_c) {
  std::string opt = arg;
  
  if (opt.size() <= 0) {
    return false;
  }

  std::vector<std::string> opts;
  boost::split(opts, arg, boost::is_any_of(","));
  if (opts.size() != 5) {
    return false;
  }
  int64 exp;
  std::stringstream ss0(opts[0]);
  ss0 >> exp;
  std::stringstream ss1(opts[1]);
  uint64 min_time_out;
  ss1 >> min_time_out;
  double ratio_y = atof(opts[2].c_str());
  double k = atof(opts[3].c_str());
  double threshold = atof(opts[4].c_str());
  boost::shared_ptr<Criterion_Exp> c_exp(new Criterion_Exp(exp));
  boost::shared_ptr<Criterion_Window> c_win(new Criterion_Window(min_time_out, ratio_y));
  stopper_c.reset(new Stopper_2C(c_exp, c_win, k, threshold));
  return true;
}

int main(int argc, char *argv[]) {
  int opt_ubound = 0;
  std::istream *ins = &std::cin;
  std::vector<std::string> other_args;
  boost::shared_ptr<Stopper> stopper_w, stopper_c;
  unsigned int srand = 1;
  bool is_rand = false;
  bool is_debug = false;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.size() == 0) {
      continue;
    }
    if (arg[0] != '-') {
      // other options
      other_args.push_back(arg);
      continue;
    } else if (arg.size() <= 1) {
      std::cerr << "missing character after \"-\"" << std::endl;
    }
    
    if (arg.substr(0,2) == "-w") {
      std::string arg1;
      if (arg.size() > 2) {
        arg1 = arg.substr(2);
      } else {
        if (i >= argc-1) {
          std::cerr << "missing argument after \"-w\"" << std::endl;
          return 1;
        }
        arg1 = argv[++i];
      }
      if (!process_arg_w(arg1.c_str(), stopper_w)) {
        std::cerr << "bad arguments for \"-w\"" << std::endl;
        return 1;
      }
      continue;
    }
    if (arg.substr(0,2) == "-c") {
      std::string arg1;
      if (arg.size() > 2) {
        arg1 = arg.substr(2);
      } else {
        if (i >= argc-1) {
          std::cerr << "missing argument after \"-c\"" << std::endl;
          return 1;
        }
        arg1 = argv[++i];
      }
      if (!process_arg_c(arg1.c_str(), stopper_c)) {
        std::cerr << "bad arguments for \"-c\"" << std::endl;
        return 1;
      }
      continue;
    }
    if (arg.substr(0,2) == "-b") {
      std::string arg1;
      if (arg.size() > 2) {
        arg1 = arg.substr(2);
      } else {
        if (i >= argc-1) {
          std::cerr << "missing argument after \"-b\"" << std::endl;
          return 1;
        }
        arg1 = argv[++i];
      }
      opt_ubound = atoi(arg1.c_str());
      continue;
    }
    if (arg.substr(0,2) == "-s") {
      std::string arg1;
      if (arg.size() > 2) {
        arg1 = arg.substr(2);
      } else {
        if (i >= argc-1) {
          std::cerr << "missing argument after \"-b\"" << std::endl;
          return 1;
        }
        arg1 = argv[++i];
      }
      srand = atol(arg1.c_str());
      continue;
    }
    if (arg == "-r") {
      is_rand = true;
      continue;
    }
    if (arg == "-d") {
      is_debug = true;
      continue;
    }
  }

  if (other_args.size() > 1) {
    std::cerr << "please specify no more than 1 input file" << std::endl;
    return 1;
  }
  std::ifstream infile;
  if (other_args.size() == 1) {
    infile.open(other_args[0].c_str());
    ins = &infile;
  }

  Clasp_Solver solver;
  solver.setInput(ins);
  solver.setSeed(srand);
  solver.setIsRand(is_rand);
  if (stopper_w.get() && stopper_c.get()) {
    std::cerr << "please specify no more than 1 stopper" << std::endl;
    return 1;
  }
  if (stopper_w.get()) {
    solver.setStopper(stopper_w);
  }
  if (stopper_c.get()) {
    solver.setStopper(stopper_c);
  }
  solver.setIsDebug(is_debug);
  try {
    if (solver.getStopper().get()) {
      solver.solve_timeout();
    } else {
      solver.solve();
    }
    std::vector<bool> model = solver.getModel();
    std::cout << "c TIME " << (double)solver.getClock().getTime()/1000 << std::endl;
    std::cout << "c SEED " << solver.getSeed() << std::endl;
    if (!solver.isSolutionFound()) {
      std::cout << "s UNSAT" << std::endl;
      return 0;
    }
    std::cout << "s SAT" << std::endl;
    std::cout << "v ";
    for (std::size_t i = 0; i < model.size(); ++i) {
      if (!model[i]) {
        std::cout << "-";
      }
      std::cout << "x" << i+1 << " ";
    }
    std::cout << std::endl;
  } catch (...) {
    std::cerr << "exception occured" << std::endl;
  }
  return 0;
}

