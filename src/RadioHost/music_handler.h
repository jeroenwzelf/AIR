#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
		std::string current_song;
};