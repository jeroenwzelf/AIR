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
    printf("generating song structure\n");
    song Song(beats);

    /* -- CASTING BAND -- */
    printf("casting and training band...\n");
    band Band(Song);
    int patchbank = 0;
    patch(instruments::acoustic_piano, 9); // drummer
	for (auto &musician : Band.band_members) {
		if (musician.instrument != instruments::no_instrument) {
			if (patchbank == 9) ++patchbank;
			if (patchbank > 0 ) patch(musician.instrument, patchbank-1);
			++patchbank;
		}
	}

	printf("performing song...\n");
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

    printf("generating new song name...\n");
    std::string name = rnd_name();
    for (unsigned i=0; i < name.length(); ++i) {
    	switch(name[i]) {
    		case '.':
    		case ',':
    		case ' ':
    		case '\"':
    		case '&':
    		case ';': name[i] = '_'; break;
    	}
    }
    std::cout << "name is " << name << std::endl;

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

std::string generateMIDI::rnd_name() {
	using namespace curlpp::options;
	std::ostringstream s;
	Json::Value tracks;
	try {
		// GET JSON OBJECT VIA API
		curlpp::Cleanup clnp;
		curlpp::Easy request;

		s << Url("http://metallizer.dk/api/json/");

		// PARSE JSON OBJECT
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(s.str().substr(20), root)) {
			std::cout << reader.getFormattedErrorMessages();
			throw;
		}
		else tracks = root["tracks"];
	}
	catch(curlpp::RuntimeError & e) {
		std::cout << e.what() << std::endl;
	}
	catch(curlpp::LogicError & e) {
		std::cout << e.what() << std::endl;
	}
	std::string name = "";
	do { name = tracks[random(0, tracks.size())].asString();
	} while (name.empty());
	return name;
}