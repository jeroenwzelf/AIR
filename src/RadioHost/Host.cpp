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
	while (true)
		update();
}

void Host::update() {
	printf("sleeping for 1 second...\n");
	std::this_thread::sleep_for(1s);

	music.update();

	if (!music.nowplaying) music.play_new_song();
	if (music.time_remaining == 10) {
		say("That was such a great song. Good to hear that band finally coming together again to create something that stunning. I am really looking forward to this next song, because it's from my favorite album that just came out 10 seconds ago. I hope you like this song just as I do. It's called, bandana banana bombana. Here we go!");
	}
}

void Host::say(std::string text) {
	//system(("lib/flite/bin/flite -voice " + vox + ' ' + '"' + ' ' + text + ' ' + '"').c_str());

	std::vector<std::string> arguments = { "lib/flite/bin/flite", "-voice", vox, ("\" " + text + " \"").c_str() };
	std::vector<char*> argv;
	for (const auto& arg : arguments)
	    argv.push_back((char*)arg.data());
	argv.push_back(nullptr);

	int pid = fork();
	if( pid < 0 ) throw;	// failed to fork
	else if (pid == 0) {
		execvp( argv[0], argv.data() );
		_Exit(EXIT_FAILURE);
	}
}