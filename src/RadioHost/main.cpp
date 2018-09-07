#include <iostream>
#include <experimental/filesystem>

#include "music_handler.h"
#include "global_info.h"
#include "stream.h"

void test_song() {
	music_handler M;
	M.play_new_song();
}

void clean_songname_file() {
	std::ofstream dislikes_file(std::string(std::getenv("HOME")) + "/airadio/status_dislikes.txt");
	std::ofstream songname_file(std::string(std::getenv("HOME")) + "/airadio/currentsong.txt");
	std::ofstream likes_file(std::string(std::getenv("HOME")) + "/airadio/status_likes.txt");
	songname_file << "N/A";
	dislikes_file << "0";
	likes_file << "0";
}

void clean_files() {
	clean_songname_file();
	std::experimental::filesystem::path path("src/RadioHost/songs/");
	if (!std::experimental::filesystem::is_empty(path))
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
			if (strcmp(argv[i], "-d") == 0) { global_info::getInstance().setIsDebugging(true); }
			if (strcmp(argv[i], "-o") == 0) { global_info::getInstance().setIsOnline(true); }
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