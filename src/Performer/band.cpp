#include "band.h"

int encode(note n) {
	switch(n) {
		case A:		return 9;	break; case Ab:	return 8;	break;
		case B:		return 11;	break; case Bb:	return 10;	break;
		case C:		return 12;	break; case F:	return 17;	break;
		case D:		return 14;	break; case Db:	return 13;	break;
		case E:		return 16;	break; case Eb:	return 15;	break;
		case G:		return 19;	break; case Gb:	return 18;	break;
		default: return 0;
	}
}

band::band(song S) : Song(S) {
	for (int i = 0; i < 7; ++i) {
		band_member musician;
		musician.part = new std::vector<Note>[Song.beats];
		band_members.push_back(musician);
	}
	give_instruments();
	// audio tech
	int chordvol, padvol;
	if (chance(50)) { padvol = 0xCC; chordvol = 0x22; }
	else { padvol = 0x33; chordvol = 0x88; }

	improv_drums(0xDD);
	improv_bass(0xCC);
	improv_chords(chordvol);
	improv_arpeggio(0xAA);
	improv_solos(0xCC);
	improv_pad(padvol);
}
// DISBAND THE BAND
band::~band() {
	for (auto musician : band_members) delete[] musician.part;
}

void band::give_instruments() {
	drummer()->instrument = instruments::acoustic_piano;
	bassist()->instrument = instruments::random_bass();
	chord_guy()->instrument = instruments::random_chord();
	arpeggio_guy()->instrument = instruments::random_melody();
	soloist_guy_1()->instrument = instruments::random_melody();
	soloist_guy_2()->instrument = instruments::random_melody();
	pad_guy()->instrument = instruments::random_pad();
}

void band::improv_drums(int vol) {
	for (unsigned i = 0; i < Song.beats; ++i) {
		if (i % 8 == 0) {
			drummer()->part[i].push_back(Note(instruments::bass_drum_1, 1, vol));
		}
		else if (i % 8 == 4) {
			drummer()->part[i].push_back(Note(instruments::acoustic_snare, 1, vol));
		}
		else drummer()->part[i].push_back(Note(-1));
	}
}

void band::improv_bass(int vol) {
	for (unsigned i = 0; i < Song.beats; ++i) {
		if (i % 8 == 0) {
			note n = Song.layout[i]->root();
			bassist()->part[i].push_back(Note( encode(n) + 12 * 3, 6, vol));
		}
		else bassist()->part[i].push_back(Note(-1));
	}
}

void band::improv_chords(int vol) {
	int playing = 0;
	for (unsigned i = 0; i < Song.beats; ++i) {
		playing--;
		if (i % 8 == 0) {
			if (playing <= 0) {
				for (auto &j : Song.layout[i]->notes) {
					playing = 6;
					chord_guy()->part[i].push_back(Note( encode(j) + 12 * 4, playing, vol ));
					//chord_guy()->part[i].push_back(Note(-1));
				}
			}
			else playing--;
		}
		chord_guy()->part[i].push_back(Note(-1));
	}
}

void band::improv_arpeggio(int vol) {
	std::vector<int> notes_to_play;
	for (unsigned i = 0; i < Song.beats; ++i) {
		unsigned j = i % 8;
		if (j == 0) {
			notes_to_play.clear();
			for (auto &note : Song.layout[i]->notes) {
				notes_to_play.push_back(encode(note));
			}
		}
		int note = j;
		if (note > 0) note = note % notes_to_play.size();
		arpeggio_guy()->part[i].push_back(Note( notes_to_play.at(note) + 12 * 3, 1, vol ));
	}
}

void band::improv_solos(int vol) {
	std::queue<int> phrase;
	bool playingphrase;
	bool playingnote = false;
	int playing_timer = 0;
	int phrases_played = 0;
	bool soloist_1 = true;
	for (unsigned i = 0; i < Song.beats; ++i) {
		if (playing_timer > 0) playing_timer--;
		else playingnote = false;

		if (phrase.empty()) { playingphrase = false; ++phrases_played; }

		if (!playingphrase && chance(20)) {
			playingphrase = true;
			for (int j = 0; j < random(1, 8); ++j) {
				note n;
				if (chance(40)) {
					if (chance(25)) n = Song.layout[i]->mode.get_step(1);
					if (chance(25)) n = Song.layout[i]->mode.get_step(3);
					if (chance(25)) n = Song.layout[i]->mode.get_step(5);
					if (chance(25)) n = Song.layout[i]->mode.get_step(7);
				}
				else n = Song.layout[i]->mode.get_step(random(1, 7));
				phrase.push(encode(n));
				if (chance(25) && phrases_played > static_cast<int>(i / 2)) soloist_1 = false;
			}
		}
		if (playingphrase && !playingnote) {
			playingnote = true;
			int n = phrase.front(); phrase.pop();
			playing_timer = random(1, 3);
			
			if (soloist_1) {
				soloist_guy_1()->part[i].push_back(Note( n + 12 * 4, playing_timer, vol ));
				soloist_guy_2()->part[i].push_back(Note(-1));
			}
			else {
				soloist_guy_1()->part[i].push_back(Note(-1));
				soloist_guy_2()->part[i].push_back(Note( n + 12 * 4, playing_timer, vol ));
			}
		}
		else {
			soloist_guy_1()->part[i].push_back(Note(-1));
			soloist_guy_2()->part[i].push_back(Note(-1));
		}
	}
}

void band::improv_pad(int vol) {
	for (unsigned i = 0; i < Song.beats; ++i) {
		if (i % 16 == 0) {
			for (auto& j : Song.layout[i]->notes) {
				pad_guy()->part[i].push_back(Note( encode(j) + 12 * 3, 16, vol ));
			}
		}
		else pad_guy()->part[i].push_back(Note(-1));
	}
}