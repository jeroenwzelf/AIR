#pragma once

#include <vector>

#include "note.h"

class scale;

namespace scales {
	enum type { IONIAN, DORIAN, PHRYGIAN, LYDIAN, MIXOLYDIAN, AEOLIAN, LOCRIAN };
	scale random_scale();
	type random_scaletype();
};

class scale {
	public:
		scale(note root);
		scale(note root, scales::type T);
		std::vector<note> notes;
		scales::type type;

		note root();
		int get_step(const note &n);	// returns step interval for note n
		note get_step(const int &i);	// returns note for step interval i
		note step_from(note n, int i);	// returns i-th step from note n
	private:
};
