#pragma once

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include <time.h>

#include "note.h"
#include "music_handler.h"

namespace hosts {
	enum name { Ryan_Elwick, Tavish_DeGroot, Michelle_Adams, Steven_Hawking };
	std::ostream &operator<<(std::ostream& s, const name& n);
}

class Host {
	public:
		Host() {}
		hosts::name name;
		void start_stream();
	protected:
		std::string vox;
		void say(std::string);
		void update();

		time_t time_started;
		time_t seconds_running;
		music_handler music;
		//stream* stream_callback;

		virtual void announce_start() = 0;
		virtual void announce_next_song() = 0;
};