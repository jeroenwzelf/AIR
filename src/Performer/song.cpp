#include "song.h"

song::song(unsigned b) : beats(b) {
	layout = new chord*[beats];
	generate_layout();
}

song::~song() { }

// for song structure (and placing each chord into a variable number of beats)
void song::generate_layout() {
	progression p(scales::random_scale());
	p.generate(beats / 16, random(0, 100));

	unsigned chord = 0;
	for (unsigned i = 0; i <  beats; i += 16) {
		for (unsigned j = 0; j < 16; ++j) {
			layout[i+j] = &p.chords.at(chord);
		}
		++chord;
	}
}