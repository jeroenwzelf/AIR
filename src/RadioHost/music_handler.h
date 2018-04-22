#pragma once

#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <stdexcept>
#include <dirent.h>
#include <stdio.h>
#include <pstream.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "note.h"
#include "generateMIDI.h"

class music_handler {
	public:
		music_handler();
		void play_new_song();
		void set_song_volume(int volume);
		void update();
		bool nowplaying;
		int time_remaining;
		~music_handler();
	private:
		std::string current_song;

		int pid_music;

		static const int MAX_SONG_HISTORY = 40;
		void remove_least_recent();
		int count_songs();
		int song_length(std::string name);
};