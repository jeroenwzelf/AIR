#include <iostream>

#include "music_handler.h"
#include "stream.h"

int main(int argc, char* argv[]) {
	if (argc > 2) {
		std::cout << "Only one extra argument is accepted!" << std::endl;
		return 0;
	}
	/* -- set flags -- */
	static const int flagcount = 1;
	bool TEST_SONG = false;
	bool GENERATE_SONG = false;
	bool ICECAST = false;
	//bool TWITCH = false;
	for (int i=1; i<1+flagcount; ++i) {
		if (argc > i) {
			if (strcmp(argv[i], "-t") == 0) TEST_SONG = true;
			if (strcmp(argv[i], "-g") == 0) GENERATE_SONG = true;
			if (strcmp(argv[i], "-icecast") == 0) ICECAST = true;
			//if (strcmp(argv[i], "-twitch") == 0) TWITCH = true;
		}
	}

	if (TEST_SONG) {
		music_handler M;
		M.play_new_song();
	}
	else if (GENERATE_SONG) {
		generateMIDI().generate(random(2*170, 2*270));
	}
	else {
		if (ICECAST) {
			system("/etc/init.d/icecast2 start");
			int pid = fork();
			if( pid < 0 ) throw;	// failed to fork
			else if (pid == 0) {
				system("darkice -c ./lib/darkice.cfg");
				_Exit(EXIT_FAILURE);
			}
		}
		//else if (TWITCH) system("./twitch_streaming.sh");
		std::cout << "Starting stream..." << std::endl;
		stream S;
		S.start_stream();
	}
	return 0;
}