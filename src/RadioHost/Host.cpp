#include "Host.h"

using namespace std::chrono_literals;

std::ostream &operator<<(std::ostream& s, const hosts::name& n) {
	switch(n) {
		case hosts::name::Ryan_Elwick:		s << "Ryan Elwick";		break;
		case hosts::name::Tavish_DeGroot:	s << "Tavish De Groot";	break;
		case hosts::name::Michelle_Adams:	s << "Michelle Adams";	break;
		case hosts::name::Steven_Hawking:	s << "Steven Hawking";	break;
	}
	return s;
}

void Host::start_stream() {
	//while (stream_callback->callback()) 
	time_started = time(0);
	music.nowplaying = true;
	music.time_remaining = 12;
	while (true)
		update();
}

void Host::update() {
	std::this_thread::sleep_for(1s);
	seconds_running = difftime( time(0), time_started );

	music.update();
	// starting up stream
	if (seconds_running < 12) {
		if (seconds_running == 1) {
			announce_start();
		}
	}
	// normal stream events
	else {
		if (!music.nowplaying) music.play_new_song();
		else if (music.time_remaining == 6) {
			announce_next_song();
		}
		else if (seconds_running > 1200) {
			time_started = time(0);
		}
	}
}

void Host::say(std::string text) {
	if (seconds_running > 12) music.set_song_volume(95);
	int pid = fork();
	if( pid < 0 ) throw;	// failed to fork
	else if (pid == 0) {
		system(("lib/flite/bin/flite -voice" + vox + "\" " + text + " \"").c_str());
		music.set_song_volume(100);
		_Exit(EXIT_FAILURE);
	}
}