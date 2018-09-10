#include "stream.h"

using namespace std::chrono_literals;

stream::stream() {
	music = new music_handler();
	host = new Host(static_cast<hosts::name>(random(0,3)), music);
}

void stream::start_stream() {
	streaming = true;

	time_started = time(0);
	music->nowplaying = true;
	music->time_remaining = 12;
	while (streaming) update();
}

void stream::update() {
	std::this_thread::sleep_for(1s);
	seconds_running = difftime( time(0), time_started );

	music->update();
	// starting up stream
	if (seconds_running < 12) {
		if (seconds_running == 1) {
			host->announce_start();
		}
	}
	// normal stream events
	else {
		if (!music->nowplaying) music->play_new_song();	// previous song has ended
		else if (music->time_remaining == 10)			// current song is almost ending
			host->announce_next_song(music->current_song);
		else if (seconds_running > 1200)				// stream has been on for 20 minutes, restart stream
			time_started = time(0);
	}
}

void stream::stop_stream() {
	streaming = false;
	music->stop_song();
}