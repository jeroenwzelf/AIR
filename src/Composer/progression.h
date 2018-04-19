#pragma once

#include <string>

#include "note.h"
#include "scale.h"
#include "chord.h"

class progression;

namespace popular_progressions {
	progression two_five_one(scale key);
	progression rythm_changes(scale key);
	progression modulating_two_five_one(scale key);
	progression chromatic_bass(scale key);
	progression the_A_train(scale key);
	progression six_one(scale key);
	progression falling_six_one(scale key);
};

class progression {
	public:
		progression(scale s);
		progression& operator+=(const progression& rhs);
		std::string print();

		void clear();
		chord* add_chord(int deg);
		chord* add_chord(chord c);
		void add_progression(progression p);
		void change_key(scale s);

		void generate(int length);
		void generate(int length, int key_change_chance);

		scale key_signature;
		std::vector<chord> chords;
	private:
		scale current_key;
		chord get_chord_degree(int deg);
};