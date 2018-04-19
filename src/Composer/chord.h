#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "note.h"
#include "scale.h"

namespace chords {
	enum type { MAJOR, MINOR, DIMINISHED, AUGMENTED };
	enum color { NO, SUS2, SUS4, MAJSEVENTH, MINSEVENTH };
};

std::ostream &operator<<(std::ostream& s, const chords::type& t);

class chord {
	public:
		chord(note root, scale s);
		chord(note root, scale s, chords::color c);
		std::vector<note> notes;

		note root();			// returns root
		std::string print();	// returns string representation of chord
		std::string print_notes();
		void set_color(chords::color c);
		int get_degree();		// returns chord degree for scale mode

		chords::type type;
		chords::color color;
		scale mode;
	private:
		void build_chord();
		void get_type();		// determines chords::type
};