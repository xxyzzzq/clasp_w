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
#include <memory>
#include <clasp/clasp_facade.h>
#include <clasp/reader.h>
#include "clasp_solver.h"
#include "stopper_window.h"
#include "stopper_2c.h"
#include "criterion_window.h"
#include "criterion_exp.h"

using namespace Clasp_W;

namespace {
std::vector<std::string> split(const std::string &str, char delim) {
  std::vector<std::string> result;
  std::string tmp;
  for (std::size_t i = 0; i < str.size(); ++i) {
    if (str[i] == delim) {
      if (!tmp.empty()) {
        result.push_back(tmp);
        tmp.clear();
      }
      continue;
    }
    tmp.push_back(str[i]);
  }
  return result;
}
}

Stopper *process_arg_w(const std::string &arg) {
  std::string opt = arg;
  if (opt == "default") {
    opt = "2000,0.9,3.0";
  }
  if (opt.size() <= 0) {
    return 0;
  }

  std::vector<std::string> opts = split(arg, ',');
  if (opts.size() != 3) {
    return 0;
  }
  std::stringstream ss0(opts[0]);
  uint64 min_time_out;
  ss0 >> min_time_out;

  return new Stopper_Window(
      min_time_out,
      atof(opts[1].c_str()),
      atof(opts[2].c_str()));
}

Stopper *process_arg_c(const std::string &arg) {
  std::string opt = arg;

  if (opt.size() <= 0) {
    return 0;
  }

  std::vector<std::string> opts = split(arg, ',');
  if (opts.size() != 5) {
    return 0;
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
  Criterion_Exp *c_exp = new Criterion_Exp(exp);
  Criterion_Window *c_win = new Criterion_Window(min_time_out, ratio_y);
  return new Stopper_2C(c_exp, c_win, k, threshold);
}

int main(int argc, char *argv[]) {
  int opt_ubound = 0;
  std::istream *ins = &std::cin;
  std::vector<std::string> other_args;
  Stopper *stopper_w = 0;  // non-owner, will be passed to ClaspSolver.
  Stopper *stopper_c = 0;  // non-owner, will be passed to ClaspSolver.
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
      stopper_w = process_arg_w(arg1);
      if (!stopper_w) {
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
      stopper_c = process_arg_c(arg1);
      if (!stopper_c) {
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
  if (stopper_w && stopper_c) {
    std::cerr << "please specify no more than 1 stopper" << std::endl;
    return 1;
  }
  if (stopper_w) {
    solver.setStopper(stopper_w);
  }
  if (stopper_c) {
    solver.setStopper(stopper_c);
  }
  solver.setIsDebug(is_debug);
  try {
    if (solver.getStopper()) {
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
