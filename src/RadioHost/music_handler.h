#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>

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
		std::string current_song_filename;
		std::string songfile_loc;
	private:
		void start_bot();
		int discordbot_pid;
};