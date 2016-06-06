#include <clasp/clasp_facade.h>
#include <iostream>
#include <sstream>

using namespace Clasp;
struct Problem1 : public Clasp::Input {
	Format format() const { return OPB; }
	bool   read(ApiPtr p, int) {
	  for (int i = 0; i < 5; ++i) {
	    std::stringstream ss;
	    ss << 'x' << i+1;
	    p.api->setAtomName(1, ss.str().c_str());
	  }
  	
	  // Define the rules of the program
	  //api.startRule(Clasp::BASICRULE).addHead(1).addToBody(2, false).endRule();
	  //api.startRule(Clasp::BASICRULE).addHead(2).addToBody(1, false).endRule();
	  p.api->startRule(Clasp::WEIGHTRULE).addToBody(1, true, 1).addToBody(2, true, 4).addToBody(5,false, 2).setBound(4).endRule();
	  p.api->startRule(Clasp::WEIGHTRULE).addToBody(1, false, 1).addToBody(2, true, 4).addToBody(5,true, 2).setBound(5).endRule();
	  p.api->startRule(Clasp::WEIGHTRULE).addToBody(2, false, 1).setBound(1).endRule();
		  return true;
	}
	// If the problem contains optimize statements, we must provide them here.
	// In our example, no optimize statements exist.
	void addMinimize(MinimizeBuilder &m, ApiPtr p) {
		wsum_t adjust = func_.adjust > 0 ? -func_.adjust : 0;
	  m.addRule(func_.lits, 
	}
	
	// Only to be used in incremental solving
	void   getAssumptions(Clasp::LitVec&) {}
};

// Callback class for reacting on events and state changes of the
// ClaspFacade.
class MyCallback : public Clasp::ClaspFacade::Callback {
public:
	typedef Clasp::ClaspFacade::Event Event;
	// Called if the current configuration contains unsafe/unreasonable options
	void warning(const char* msg) { std::cout << "Warning: " << msg << std::endl; }
	// Called on entering/exiting a state
	void state(Event, Clasp::ClaspFacade&)     { }
	// Called for important events, e.g. a model has been found
	void event(const Clasp::Solver& s, Event e, Clasp::ClaspFacade& f) {
		if (e == Clasp::ClaspFacade::event_model) {
			std::cout << "Model " << f.config()->ctx.enumerator()->enumerated << ": \n";
			const Clasp::SymbolTable& symTab = s.sharedContext()->symTab();
			for (Clasp::SymbolTable::const_iterator it = symTab.begin(); it != symTab.end(); ++it) {
				if (s.isTrue(it->second.lit) && !it->second.name.empty()) {
					std::cout << it->second.name.c_str() << " ";
				}
			}
			std::cout << std::endl;
		}
	}
};

void example2() {
	// The "interface" to the clasp library
	Clasp::ClaspFacade libclasp;
	// Our input class
	Problem            problem;
	// Our class for printing models etc.
	MyCallback         cb;
	
	// Aggregates configuration options.
	// Using config, you can control many parts of the search, e.g.
	// - the amount and kind of preprocessing
	// - the enumerator to use and the number of models to compute
	// - the heuristic used for decision making
	// - the restart strategy
	// - ...
	Clasp::ClaspConfig config;

	// We want to compute all models but
	// otherwise we use the default configuration
	config.enumerate.numModels = 0;
	
	// Solve the problem using the given configuration.
	// Report status via our callback interface
	libclasp.solve(problem, config, &cb);
	
	std::cout << "No more models!" << std::endl;
}
