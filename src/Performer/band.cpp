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
	else { padvol = 0x33; chordvol = 0x33; }

	improv_drums(0xDD);
	improv_bass(0xCC);
	improv_chords(chordvol);
	improv_arpeggio(0xDD);
	improv_solos(0xCC);
	improv_pad(padvol);
}
// DISBAND THE BAND
band::~band() {
	for (auto musician : band_members) delete[] musician.part;
}

void band::give_instruments() {
	switch(random(0, 10)) {
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
					style_random(); break;
		case 8:		style_piano();	break;
		case 9:		style_guitar();	break;
		case 10:	style_8bit();	break;
	}
}

void band::style_random() {
	if (random(0, 100) > 50)
		drummer()->instrument = instruments::acoustic_piano;
	else drummer()->instrument = instruments::no_instrument;

	bassist()->instrument = instruments::random_bass();

	if (random(0, 100) > 40)
		chord_guy()->instrument = instruments::random_chord();
	else chord_guy()->instrument = instruments::no_instrument;

	if (random(0, 100) > 30)
		arpeggio_guy()->instrument = instruments::random_melody();
	else arpeggio_guy()->instrument = instruments::no_instrument;

	soloist_guy_1()->instrument = instruments::random_melody();
	soloist_guy_2()->instrument = instruments::random_melody();

	if (random(0, 100) > 20)
		pad_guy()->instrument = instruments::random_pad();
	else pad_guy()->instrument = instruments::no_instrument;
}

void band::style_piano() {
	drummer()->instrument = instruments::no_instrument;
	bassist()->instrument = instruments::acoustic_piano;
	chord_guy()->instrument = instruments::acoustic_piano;
	arpeggio_guy()->instrument = instruments::acoustic_piano;
	soloist_guy_1()->instrument = instruments::acoustic_piano;
	soloist_guy_2()->instrument = instruments::acoustic_piano;
	pad_guy()->instrument = instruments::no_instrument;
}

void band::style_guitar() {
	drummer()->instrument = instruments::no_instrument;
	bassist()->instrument = instruments::acoustic_guitar_nylon;
	chord_guy()->instrument = instruments::no_instrument;
	arpeggio_guy()->instrument = instruments::acoustic_guitar_nylon;
	soloist_guy_1()->instrument = instruments::acoustic_guitar_nylon;
	soloist_guy_2()->instrument = instruments::no_instrument;
	pad_guy()->instrument = instruments::no_instrument;
}

void band::style_8bit() {
	if (random(0, 100) > 70)
		drummer()->instrument = instruments::no_instrument;
	else drummer()->instrument = instruments::acoustic_piano;
	bassist()->instrument = instruments::synth_bass_1;
	chord_guy()->instrument = instruments::no_instrument;
	arpeggio_guy()->instrument = instruments::lead_square;
	soloist_guy_1()->instrument = instruments::lead_sawtooth;
	soloist_guy_2()->instrument = instruments::lead_bass;
	pad_guy()->instrument = instruments::random_pad();
}

/*drum_sequence band::improv_drums_sequence(int vol) {
	std::ifstream file("drumsequences");
	std::string str;
    std::array<std::string, 8> sequencelist; 
    for (unsigned i = 0; i < 8; ++i) std::getline(file, sequencelist[i]);

    unsigned it = 0;
    drum_sequence sequence;
	for (auto &pair : sequence) {
		for (int i = 0; i < 16; ++i) {
			sequence.second = sequencelist[it].
		}
	}
}*/

void band::improv_drums(int vol) {
	if (drummer()->instrument != instruments::no_instrument) {
		if (chance(20)) {
			for (unsigned i = 0; i < Song.beats; ++i) {
				if (i < 64) drummer()->part[i].push_back(Note(-1));
				if (i % 8 == 0) {
					drummer()->part[i].push_back(Note(instruments::bass_drum_1, 1, vol));
				}
				else if (i % 8 == 4) {
					drummer()->part[i].push_back(Note(instruments::acoustic_snare, 1, vol));
				}
				else drummer()->part[i].push_back(Note(-1));
			}
		}
		if (chance(30)) {
			bool hats = static_cast<bool>(random(0,1));
			for (unsigned i = 0; i < Song.beats; ++i) {
				if (i % 16 == 0 && chance(10)) hats = !hats;
				if (hats) {
					if (i % 8 == 6) drummer()->part[i].push_back(Note(instruments::open_hi_hat, 1, vol));
					else if (i % 2 == 0) drummer()->part[i].push_back(Note(instruments::closed_hi_hat, 1, vol));
				}
				if (i % 8 == 0 || i % 8 == 3 || i % 8 == 5 || i % 8 == 7) {
					drummer()->part[i].push_back(Note(instruments::bass_drum_1, 1, vol));
				}
				if (i % 8 == 2 || i%8 == 6) {
					drummer()->part[i].push_back(Note(instruments::electric_snare, 1, vol));
				}
				else drummer()->part[i].push_back(Note(-1));
			}
		}
		else if (chance(30)) {
			for (unsigned i = 0; i < Song.beats; ++i) {
				if (i % 8 == 0 || i % 8 == 3 || i % 8 == 7) {
					drummer()->part[i].push_back(Note(instruments::bass_drum_1, 1, vol));
				}
				else if (i % 8 == 4) {
					drummer()->part[i].push_back(Note(instruments::acoustic_snare, 1, vol));
				}
				if (chance (70)) {
					drummer()->part[i].push_back(Note(static_cast<instruments::drums>(random(35, 46)), 1, vol));
				}
				else drummer()->part[i].push_back(Note(-1));
			}
		}
		else {
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
	}
	else {
		for (unsigned i = 0; i < Song.beats; ++i)
			drummer()->part[i].push_back(Note(-1));
		drummer()->instrument = instruments::acoustic_piano;
	}
}

void band::improv_bass(int vol) {
	if (bassist()->instrument != instruments::no_instrument) {
		for (unsigned i = 0; i < Song.beats; ++i) {
			if (i % 8 == 0) {
				note n = Song.layout[i]->root();
				bassist()->part[i].push_back(Note( encode(n) + 12 * 3, 6, vol));
			}
			else if (chance(20)) {
				note n = Song.layout[i]->notes.at(random(0, Song.layout[i]->notes.size()-1));
				bassist()->part[i].push_back(Note( encode(n) + 12 * 3, 6, vol));
			}
			if (i % 8 == 6 && chance(30)) {
				note n = Song.layout[i]->notes.at(Song.layout[i]->notes.size()-2);
				bassist()->part[i].push_back(Note( encode(n) + 12 * 3, 6, vol));
			}
			else bassist()->part[i].push_back(Note(-1));
		}
	}
	else {
		for (unsigned i = 0; i < Song.beats; ++i)
			bassist()->part[i].push_back(Note(-1));
	}
}

void band::improv_chords(int vol) {
	if (chord_guy()->instrument != instruments::no_instrument) {
		int playing = 0;
		for (unsigned i = 0; i < Song.beats; ++i) {
			playing--;
			if (i % 8 == 0) {
				if (playing <= 0) {
					for (auto &j : Song.layout[i]->notes) {
						playing = 6;
						chord_guy()->part[i].push_back(Note( encode(j) + 12 * 4, playing, vol ));
					}
				}
				else playing--;
			}
			chord_guy()->part[i].push_back(Note(-1));
		}
	}
	else {
		for (unsigned i = 0; i < Song.beats; ++i)
			chord_guy()->part[i].push_back(Note(-1));
	}
}

void band::improv_arpeggio(int vol) {
	if (arpeggio_guy()->instrument != instruments::no_instrument) {
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
	else {
		for (unsigned i = 0; i < Song.beats; ++i)
			arpeggio_guy()->part[i].push_back(Note(-1));
	}
}

void band::improv_solos(int vol) {
	if (soloist_guy_1()->instrument != instruments::no_instrument
		&& soloist_guy_2()->instrument != instruments::no_instrument) {
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

			if (!playingphrase && chance(8)) {
				playingphrase = true;
				int phraselength = random(1, 8);
				for (int j = 0; j < phraselength; ++j) {
					note n;
					if (chance(40)) {
						scale s(Song.layout[i]->root(), scales::random_scaletype());
						if (chance(25)) n = s.get_step(1);
						else if (chance(25)) n = s.get_step(3);
						else if (chance(50)) n = s.get_step(5);
						else if (chance(25)) n = s.get_step(7);
						else n = s.get_step(random(1, 7));
					}
					if (j-1 == phraselength) {
						if (chance(40)) n = Song.layout[i]->mode.get_step(1);
						else if (chance(25)) n = Song.layout[i]->mode.get_step(3);
						else if (chance(25)) n = Song.layout[i]->mode.get_step(5);
						else n = Song.layout[i]->mode.get_step(random(1, 5));
					}
					else if (chance(70)) {
						if (chance(25)) n = Song.layout[i]->mode.get_step(1);
						else if (chance(25)) n = Song.layout[i]->mode.get_step(3);
						else if (chance(25)) n = Song.layout[i]->mode.get_step(5);
						else if (chance(25)) n = Song.layout[i]->mode.get_step(7);
						else n = Song.layout[i]->mode.get_step(random(1, 5));
					}
					else n = Song.layout[i]->mode.get_step(random(1, 7));
					phrase.push(encode(n));
					if (chance(25) && i > static_cast<unsigned>(3 * Song.beats / 4)) soloist_1 = false;
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
	else {
		for (unsigned i = 0; i < Song.beats; ++i) {
			soloist_guy_1()->part[i].push_back(Note(-1));
			soloist_guy_2()->part[i].push_back(Note(-1));
		}
	}
}

void band::improv_pad(int vol) {
	if (pad_guy()->instrument != instruments::no_instrument) {
		for (unsigned i = 0; i < Song.beats; ++i) {
			if (i % 16 == 0) {
				for (auto& j : Song.layout[i]->notes) {
					pad_guy()->part[i].push_back(Note( encode(j) + 12 * 3, 16, vol ));
				}
			}
			else pad_guy()->part[i].push_back(Note(-1));
		}
	}
	else {
		for (unsigned i = 0; i < Song.beats; ++i)
			pad_guy()->part[i].push_back(Note(-1));
	}
}