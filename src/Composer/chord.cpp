#include "chord.h"

std::ostream &operator<<(std::ostream& s, const chords::type& t) {
	switch(t) {
		case chords::MINOR: s << "m"; break;
		case chords::DIMINISHED: s << "dim"; break;
		case chords::AUGMENTED: s << "+"; break;
		default: break;
	}
	return s;
}

chord::chord(note root, scale s) : mode(s) {
	notes.push_back(root);
	build_chord();
}

chord::chord(note root, scale s, chords::color c) : mode(s) {
	notes.push_back(root);
	build_chord();
	set_color(c);
}

void chord::build_chord() {
	while (notes.size() > 1) notes.pop_back();
	note n = root();
	notes.push_back(mode.step_from(n, 3));
	notes.push_back(mode.step_from(n, 5));
	get_type();
}

void chord::get_type() {
	int interval1 = interval(notes.at(0), notes.at(1));
	int interval2 = interval(notes.at(1), notes.at(2));
	if (interval1 == 3) {
		if (interval2 == 3) type = chords::DIMINISHED;
		else if (interval2 == 4) type = chords::MINOR;
	}
	else if (interval1 == 4) {
		if (interval2 == 3) type = chords::MAJOR;
		else if (interval2 == 4) type = chords::AUGMENTED;
	}
}

void chord::set_color(chords::color c) {
	switch(c) {
		case chords::SUS2: notes.at(1) -= 1; break;
		case chords::SUS4: notes.at(1) += 1; break;
		case chords::MAJSEVENTH: notes.push_back(root()-1); break;
		case chords::MINSEVENTH: notes.push_back(root()-2); break;
		default: break;
	}
	color = c;
}

int chord::get_degree() {
	return mode.get_step(root());
}

note chord::root() { return notes.at(0); }

std::string chord::print() {
	std::ostringstream out;
	out << root() << type;
	return out.str();
}

std::string chord::print_notes() {
	std::ostringstream out;
	out << "( ";
	for (note &n : notes) out << n << " ";
	out << ")";
	return out.str();
}