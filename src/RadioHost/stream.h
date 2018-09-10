#pragma once

#include <algorithm>

#include "note.h"
#include "Host.h"
#include "music_handler.h"

class stream {
	public:
		stream();
		void start_stream();
		void stop_stream();
	private:
		void update();
		bool streaming;
		time_t time_started;
		time_t seconds_running;
		
		Host* host;
		music_handler* music;
};