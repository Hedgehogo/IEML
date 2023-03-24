#include "Mark.hpp"

namespace ieml {
	void Mark::enter() {
		symbol = 0;
		++line;
	}
	
	void Mark::enter(String::const_iterator& pos) {
		++pos;
		enter();
	}
}