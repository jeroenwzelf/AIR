#include <iostream>

#include "music_handler.h"
#include "stream.h"

void test_song() {
	music_handler M;
	M.play_new_song();
}

void clean_songname_file() {
	std::ofstream songname_file(std::string(std::getenv("HOME")) + "/AIRADIO_currentsong.txt");
	songname_file << "N/A";
}

void clean_files() {
	clean_songname_file();
	system("exec rm -r src/RadioHost/songs/*");
}

int main(int argc, char* argv[]) {
	if (argc > 2) {
		std::cout << "Only one extra argument is accepted!" << std::endl;
		return 0;
	}
	/* -- set flags -- */
	static const int flagcount = 1;
	bool NO_FLAGS = true;
	for (int i=1; i<1+flagcount; ++i) {
		if (argc > i) {
			if (strcmp(argv[i], "-t") == 0) { test_song(); NO_FLAGS = false; }
			if (strcmp(argv[i], "-g") == 0) { generateMIDI().generate(random(2*170, 2*270)); NO_FLAGS = false; }
			if (strcmp(argv[i], "-c") == 0) { clean_files(); NO_FLAGS = false; }
		}
	}
	if (NO_FLAGS) {
		clean_songname_file();
		printf("Starting stream...\n");
		stream S;
		S.start_stream();
	}
	return 0;
}