#pragma once

#include <queue>

#include "note.h"
#include "scale.h"
#include "chord.h"
#include "progression.h"
#include "song.h"
#include "midi_instruments.h"

struct band_member {
	std::vector<Note>* part;
	instruments::name instrument;
};

class band {
	public:
		band(song S);
		~band();
		// band members:
		std::vector<band_member> band_members;
		band_member* drummer() { return &band_members.at(0); }
		band_member* bassist() { return &band_members.at(1); }
		band_member* chord_guy() { return &band_members.at(2); }
		band_member* arpeggio_guy() { return &band_members.at(3); }
		band_member* soloist_guy_1() { return &band_members.at(4); }
		band_member* soloist_guy_2() { return &band_members.at(5); }
		band_member* pad_guy() { return &band_members.at(6); }
	private:
		song Song;
		void give_instruments();
		// band members improvise all parts below
		void improv_drums(int vol);
		void improv_bass(int vol);
		void improv_chords(int vol);
		void improv_arpeggio(int vol);
		void improv_solos(int vol);
		void improv_pad(int vol);
};