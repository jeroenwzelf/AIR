#include "scale.h"

scale scales::random_scale() {
	return scale(random_note(), scales::random_scaletype());
}

scales::type scales::random_scaletype() {
	return static_cast<scales::type>(random(0, 6));
}

scale::scale(note root, scales::type T) : type(T) {
	notes.push_back(root);
	switch(T) {
		case scales::IONIAN: {
			notes.push_back(root + 2);
			notes.push_back(root + 4);
			notes.push_back(root + 5);
			notes.push_back(root + 7);
			notes.push_back(root + 9);
			notes.push_back(root + 11);
		} break;
		case scales::DORIAN: {
			notes.push_back(root + 2);
			notes.push_back(root + 3);
			notes.push_back(root + 5);
			notes.push_back(root + 7);
			notes.push_back(root + 9);
			notes.push_back(root + 10);
		} break;
		case scales::PHRYGIAN: {
			notes.push_back(root + 1);
			notes.push_back(root + 3);
			notes.push_back(root + 5);
			notes.push_back(root + 7);
			notes.push_back(root + 8);
			notes.push_back(root + 10);
		} break;
		case scales::LYDIAN: {
			notes.push_back(root + 2);
			notes.push_back(root + 4);
			notes.push_back(root + 6);
			notes.push_back(root + 7);
			notes.push_back(root + 9);
			notes.push_back(root + 11);
		} break;
		case scales::MIXOLYDIAN: {
			notes.push_back(root + 2);
			notes.push_back(root + 4);
			notes.push_back(root + 5);
			notes.push_back(root + 7);
			notes.push_back(root + 9);
			notes.push_back(root + 10);
		} break;
		case scales::AEOLIAN: {
			notes.push_back(root + 2);
			notes.push_back(root + 3);
			notes.push_back(root + 5);
			notes.push_back(root + 7);
			notes.push_back(root + 8);
			notes.push_back(root + 10);
		} break;
		case scales::LOCRIAN: {
			notes.push_back(root + 1);
			notes.push_back(root + 3);
			notes.push_back(root + 5);
			notes.push_back(root + 6);
			notes.push_back(root + 8);
			notes.push_back(root + 10);
		} break;
	}
}

note scale::root() { return notes.at(0); }
note scale::get_step(const int &i) { return notes.at( (i-1) % notes.size() ); }
int scale::get_step(const note &n) {
	int i=0;
	for (auto &N : notes) {
		if (N == n) return i;
		else i = (i + 1) % notes.size();
	}
	return -1;
}
note scale::step_from(note n, int i) {
	int deg = get_step(n);
	if (deg != -1) return get_step(deg+i);
	return n;
}