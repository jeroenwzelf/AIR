#include "generateMIDI.h"

int generateMIDI::encode(note n) {
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

unsigned generateMIDI::get_patch_bank(instruments::name n) {
	unsigned i=0;
	for (; i<16; ++i) {
		if (i != 9 && patch_bank[i] == n) { 
			return i;
		}
	}
	return i;
}

void generateMIDI::patch(instruments::name n, unsigned patch) {
	if (patch >= 0 && patch < 16) {
		std::cout << "putting " << n << " in patch bank " << patch << std::endl;
		patch_bank[patch] = n;
		file[0].Patch(patch, n);
	}
}

void generateMIDI::play(int note, unsigned duration, unsigned patch, int vol) {
	if (note > 0) {
		Note N(note, duration, patch, vol);
		file[0].KeyOn(N.patch_bank, N.note, N.vol);
		keys_on.push_back(N);
	}
}

auto generateMIDI::stop(auto it) {
	file[0].KeyOff(it->patch_bank, it->note, 0xDD);
	return keys_on.erase(it);
}

void generateMIDI::update_notes() {
	for (auto it = keys_on.begin(); it != keys_on.end(); ) {
		if (--(it->duration) == 0) it = stop(it);
		else ++it;
	}
}

float BPM_to_delay(float bpm) { return (-1.14 * bpm + 285.5); }
std::string generateMIDI::generate(int songsize) {
	std::cout << "generating .midi file..." << std::endl;
    file.AddLoopStart();

    /* -- TEMPO, LENGTH, TIME SIGNATURE (4/4) -- */
    // decide tempo
    float bpm = random(75, 175);
    float bps = bpm / 60.0f;
    float delay = BPM_to_delay(bpm);

    // decide amount of beats depending on tempo and song length
    // always ensures amount of beats is divisible by 16
    unsigned beats = ( (static_cast<int>(bps * songsize) - 1 * 4) | 15 ) + 1;

    /* -- ASK COMPOSER TO WRITE SONG -- */
    song Song(beats);

    /*std::cout << "beats: " << beats << std::endl
    			<< "bpm: " << bpm << std::endl
    			<< "arrangement: " << Song.p.chords.size() << " chords. " << std::endl;*/

    /* -- CASTING BAND -- */
    band Band(Song);
    int patchbank = 0;
    patch(Band.drummer()->instrument, 9);
	for (auto &musician : Band.band_members) {
		if (patchbank > 0) patch(musician.instrument, patchbank-1);
		++patchbank;
	}

    /* -- BAND PLAYS ALL PARTS TOGETHER -- */
    for (unsigned i=0; i<beats; ++i) {
    	update_notes();

    	bool drummer = true;
		for (auto &musician : Band.band_members) {
			for (auto &N : musician.part[i]) {
				if (drummer) { play(N.note, N.duration, drums_patch, N.vol); drummer = false; }
				else play(N.note, N.duration, get_patch_bank(musician.instrument), N.vol);
			}
		}

	    file[0].AddDelay(delay);
    }

    std::string name = "newest";

    file.AddLoopEnd();
    file.Finish();
    FILE* fp = std::fopen(("src/RadioHost/songs/" + name + ".mid").c_str(), "wb");
    std::fwrite(&file.at(0), 1, file.size(), fp);
    std::fclose(fp);

    std::cout << "converting .midi to .wav..." << std::endl;
    system(("timidity src/RadioHost/songs/" + name + ".mid -Ow").c_str());

    std::cout << "removing .midi file..." << std::endl;
    system(("rm src/RadioHost/songs/" + name + ".mid").c_str());

    return name;
}