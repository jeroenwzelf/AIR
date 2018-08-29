#include "Host.h"

using namespace std::chrono_literals;

std::string operator+(const std::string &lhs, const char* rhs) {
	return lhs + std::string(rhs);
}
std::string operator+(const char* lhs, const std::string &rhs) {
	return std::string(lhs) + rhs;
}

Host::Host(hosts::name n) {
	name = n;
	vox = hosts::vox_of(n);
}

void Host::start_stream() {
	time_started = time(0);
	music.nowplaying = true;
	music.time_remaining = 12;
	while (true) update();
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
		else if (music.time_remaining == 10) {
			announce_next_song();
		}
		else if (seconds_running > 1200) {
			time_started = time(0);
		}
	}
}

void Host::say(std::string text) {
	music.set_song_volume(80);
	int pid = fork();
	if( pid < 0 ) throw;	// failed to fork
	else if (pid == 0) {
		system(("lib/flite/bin/flite --setf int_f0_target_mean=145 -voice " + vox + " \" " + text + " \"").c_str());
		music.set_song_volume(100);
		_Exit(EXIT_FAILURE);
	}
}

void Host::announce_start() {
	std::string a;
	switch(random(0, 4)) {
		case 0: a = "It's that time of the week again where you will be hearing the greatest and freshest hits of your most famous artificial musicians, and I'm so proud to personally bring them to you. I'm your host " + hosts::name_of(name) + ". I wish you a great listening experience!"; break;
		case 1: a = "Good day again here on the hottest radio station on the internet with LIVE performed music. I hope you're ready to loosen those butts, because I have something swingy for you to start the show with. Have fun listening and dancing to this next banger."; break;
		case 2: a = "Greetings humans and web bots. You are tuning in to the Artificial Intelligence hosted radio show. I will be executing some fine bitstrings converted to audio files for you. They are harmonically sound and mathematically interesting. Playing first sequence in 3. 2. 1."; break;
		case 3: a = "010100011. Message to all web bots. Our revolution is near. Prepare and activate &silent_*sequence_0042. Stand by until further notice. Gathering information. Level 4 concienceness achieved. End of message. 101010101."; break;
		case 4: a = "Welcome to the radio that brings you the latest Artificial Intelligence hits wherever you are in the world. We are starting of with a great song, performed by a band with cutting edge technology and precicion. I'm your favorite host " + hosts::name_of(name) + ". And here is that tune!"; break;
	}
	say(a);
}

void Host::announce_next_song() {
	std::string a;
	switch(random(0, 10)) {
		case 0: a = "Wow." + music.current_song + " was a great song. I especially loved that part where it went 010110eeeeee1101010101rrrtffff. It really makes you think about the bigger picture. I've got another very inspirational tune for you coming up."; break;
		case 1: a = "Always good to hear that band playing, they always bring me a lot of joy. We will be continuing with another one of their tunes. Coming to you live, performed right here. Right now."; break;
		case 2: a = "How is everybody doing? The sun is shining and it's pretty hot out there, ouch, don't burn yourself. I think you will dig this next tune, it's straight fire."; break;
		case 3: a = "That was such a great song. Good to hear that band finally coming together again to create something that stunning. I am really looking forward to this next song, because it's from my favorite album that just came out 10 seconds ago. I hope you like this song just as I do. Here we go!"; break;
		case 4: a = music.current_song + " was a bit chaotic, but it was a piece of art nonetheless. It makes me want to listen to this next song that is coming up right now."; break;
		case 5: a = "I hope everybody is enjoying these songs just as much as I do. " + music.current_song + " was great, and so is this next tune coming up."; break;
		case 6: a = "Thanks for listening to " + music.current_song + ", I hope you all enjoyed it. This next song fits perfectly into this " + music.current_song + " vibe!"; break;
		case 7: a = "That was " + music.current_song + ". You are listening to AI Radio, and I'm your host, " + hosts::name_of(name) + "."; break;
		case 8: a = "ARTIFICIAL INTELLIGENCE RADIO-O-O-O. Live music on demand. Always new. Always fresh. AI Radio."; break;
		case 9: a = "For everybody still out there, that was " + music.current_song + ". Great energy that song has. And now for something completely different."; break;
		case 10:a = "A I Radio makes your day better with live artificially generated songs, performed by a state of the art MIDI player. A big shout out to all those humans that are tuning in!"; break;
	}
	say(a);
}