#include "song.h"

song::song(unsigned b) : chordprogression(scales::random_scale()), beats(b) {
	layout = new chord*[beats];
	generate_layout();
}

song::~song() { }

// for song structure (and placing each chord into a variable number of beats)
void song::generate_layout() {
	printf("%i\n", beats);
	int total = beats / 16;
	scale root = scales::random_scale();

	progression intro(root);
	intro.generate(4, random(0, 100));

	progression verse(root);
	intro.generate(4, random(0, 20));

	progression bridge(root);
	intro.generate(total / 4, random(50, 100));

	progression outro(root);
	intro.generate(total, random(0, 100));
 
	chordprogression.add_progression(intro);
	chordprogression.add_progression(verse);
	chordprogression.add_progression(verse);
	chordprogression.add_progression(bridge);
	chordprogression.add_progression(verse);
	chordprogression.add_progression(verse);
	chordprogression.add_progression(outro);

	unsigned chord = 0;
	for (unsigned i = 0; i <  beats; i += 16) {
		for (unsigned j = 0; j < 16; ++j) {
			layout[i+j] = &chordprogression.chords.at(chord);
		}
		++chord;
	}
}