#include "progression.h"

progression::progression(scale s) : key_signature(s), current_key(s) {}

progression& progression::operator+=(const progression& rhs) {
	for (auto c : rhs.chords) this->add_chord(c);
	this->current_key = rhs.current_key;
	return *this;
}
void progression::clear() { chords.clear(); current_key = key_signature; }
chord* progression::add_chord(int deg) { chords.push_back(get_chord_degree(deg)); return &chords.back(); }
chord* progression::add_chord(chord c) { chords.push_back(c); return &chords.back(); }
void progression::add_progression(progression p) { *this += p; }
void progression::change_key(scale s) { current_key = s; }

chord progression::get_chord_degree(int deg) {
	return chord(current_key.get_step(deg), current_key);
}

std::string progression::print() {
	std::ostringstream out;
	for (auto &c : chords) {
		int deg = c.get_degree();
		if (deg != -1) {
			out << c.print() << ": " << c.print_notes() << " (";
			switch(deg) {
				case 0: out << "I"; break;
				case 1: out << "II"; break;
				case 2: out << "III"; break;
				case 3: out << "IV"; break;
				case 4: out << "V"; break;
				case 5: out << "VI"; break;
				case 6: out << "VII"; break;
			}
			out << ")" << std::endl;
		}
	}
	return out.str();
}

void progression::generate(int length) { generate(length, 0); }
/*void progression::generate(int length, int key_change_chance) {
	for (int i=0; i < length; ++i) {
		add_chord(1);
	}
}*/
void progression::generate(int length, int key_change_chance) {
	clear();
	int i = length;
	while (i > 0) {
		if (chance(20)) {
			if (i >= 3 && chance(10)) {
				add_progression(popular_progressions::two_five_one(current_key));
				i-=3;
			}
			else if (i >= 4 && chance(1)) {
				add_progression(popular_progressions::six_one(current_key));
				i-=4;
			}
			else if (i >= 5 && chance(1)) {
				add_progression(popular_progressions::chromatic_bass(current_key));
				i-=5;
			}
			else if (i >= 6 && chance(1)) {
				add_progression(popular_progressions::falling_six_one(current_key));
				i-=6;
			}
			else if (i >= 7 && chance(10)) {
				if (chance(50)) {
					add_progression(popular_progressions::rythm_changes(current_key));
					i-=7;
				}
				else if (chance(50) && chance(key_change_chance)) {
					add_progression(popular_progressions::modulating_two_five_one(current_key));
					change_key(scale(current_key.root() - 4, scales::random_scaletype()));
					i-=7;
				}
				else if (chance(50)) {
					add_progression(popular_progressions::the_A_train(current_key));
					i-=7;
				}
			}
		}
		else {
			add_chord(random(1, 7));
			i--;
		}
		if (!chords.empty() && key_change_chance > 0 && chance(key_change_chance)) {
			change_key(scale(chords.back().root(), scales::random_scaletype()));
			key_change_chance -= 5;
		}
	}
}


/* --- ALL POPULAR PROGRESSIONS --- */

progression popular_progressions::two_five_one(scale key) {
	progression p(key);
	p.add_chord(2); p.add_chord(5); p.add_chord(1);
	return p;
}

progression popular_progressions::rythm_changes(scale key) {
	//scale s(key.root(), scales::IONIAN);
	progression p(key);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);
	p.add_chord(6)->set_color(chords::MINSEVENTH);
	p.add_chord(2)->set_color(chords::MINSEVENTH);
	p.add_chord(5)->set_color(chords::MINSEVENTH);
	p.add_chord(3)->set_color(chords::MINSEVENTH);
	p.add_chord(6)->set_color(chords::MINSEVENTH);
	p.add_chord(2)->set_color(chords::MINSEVENTH);
	p.add_chord(5)->set_color(chords::MINSEVENTH);
	return p;
}

progression popular_progressions::modulating_two_five_one(scale key) {
	scale modulated = key;
	progression p(modulated);
	for (int i=0; i<2; ++i) {
		progression TFO(modulated);
		TFO.add_chord(1)->set_color(chords::SUS2);
		TFO.add_chord(1)->set_color(chords::MAJSEVENTH);
		TFO.add_chord(2)->set_color(chords::MINSEVENTH);
		TFO.add_chord(5)->set_color(chords::MINSEVENTH);
		modulated = scale(modulated.root()-2, modulated.type);
		p += TFO;
	}
	return p;
}

progression popular_progressions::chromatic_bass(scale key) {
	key = scale(key.root(), scales::IONIAN);
	progression p1(key);
	p1.add_chord(1)->set_color(chords::MAJSEVENTH);

	scale s(key.root()-1, scales::AEOLIAN);
	progression p2(s);
	p2.add_chord(2)->set_color(chords::MINSEVENTH);

	p1 += p2;
	p1.change_key(key);
	p1.add_chord(2)->set_color(chords::MINSEVENTH);

	s = scale(key.root()+1, scales::AEOLIAN);
	p2 = progression(s);
	p2.add_chord(2)->set_color(chords::MINSEVENTH);

	p1 += p2;
	p1.change_key(key);
	p1.add_chord(3)->set_color(chords::MINSEVENTH);
	p1.add_chord(6)->set_color(chords::MAJSEVENTH);
	return p1;
}

progression popular_progressions::the_A_train(scale key) {
	progression p(key);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);

	scale s(key.root()+2, key.type);
	progression p2(s);
	p2.add_chord(1)->set_color(chords::MINSEVENTH);
	p2.add_chord(1)->set_color(chords::MINSEVENTH);

	p += p2;
	p.change_key(key);
	p.add_chord(2)->set_color(chords::MINSEVENTH);
	p.add_chord(5)->set_color(chords::MINSEVENTH);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);
	return p; 
}

progression popular_progressions::six_one(scale key) {
	progression p(key);
	p.add_chord(1)->set_color(chords::MAJSEVENTH);
	p.add_chord(2)->set_color(chords::MINSEVENTH);
	p.add_chord(5)->set_color(chords::MINSEVENTH);
	p.add_chord(6)->set_color(chords::MAJSEVENTH);
	return p;
}

progression popular_progressions::falling_six_one(scale key) {
	progression p(key);
	p.add_chord(6);
	scale third(key.root()+4, scales::IONIAN);
	progression p2(third);
	p2.add_chord(1);

	p += p2;
	p.change_key(key);
	p.add_chord(5);

	scale second(key.root()+2, scales::IONIAN);
	progression p3(second);
	p3.add_chord(1);

	p += p3;
	p.change_key(key);
	p.add_chord(4);
	p.add_chord(5);
	return p;
}