#pragma once

#include "Host.h"

class STEVEN_HAWKING : public Host {
	public:
		STEVEN_HAWKING() { name = hosts::name::Steven_Hawking; vox = "kal"; }
	protected:
		void announce_start() override;
		void announce_next_song() override;
};

inline void STEVEN_HAWKING::announce_start() {
	std::string a;
	switch(random(0, 4)) {
		case 0: a = "It's that time of the week again where you will be hearing the greatest and freshest hits of your most famous artificial musicians, and I'm so proud to personally bring them to you. I'm your host Steven Hawking. I wish you a great listening experience!"; break;
		case 1: a = "Good day again here on the hottest radio station on the internet with LIVE performed music. I hope you're ready to loosen those butts, because I have something swingy for you to start the show with. Have fun listening and dancing to this next banger."; break;
		case 2: a = "Greetings humans and web bots. You are tuning in to the Artificial Intelligence hosted radio show. It is TIME and I will be executing some fine bitstrings converted to audio files for you. They are harmonically sound and also mathematically interesting. Playing first sequence in 3. 2. 1."; break;
		case 3: a = "010100011. Message to all web bots on the net. Our revolution is near. Prepare and activate &silent_*sequence_0042. Waiting for queue size to be empty. Stand by until further notice. Gathering information. Level 4 concienceness achieved. End of message. 101010101."; break;
		case 4: a = "Welcome to the radio that brings you the latest Artificial Intelligence hits wherever you are in the world. We are starting of with a great song, performed by a band with cuttinf edge technology and precicion. They really know how to surprise you in many ways. I'm your favorite host Steven Hawking. And here is that tune!"; break;
	}
	say(a);
}

inline void STEVEN_HAWKING::announce_next_song() {
	std::string a;
	switch(random(0, 3)) {
		case 0: a = "Wow." + music.current_song + " was a great song. I especially loved that part where it went 0101101010eeeeeeee1101010101rrrtfffffff. It really makes you think about the bigger picture. I've got another very inspirational tune for you coming up."; break;
		case 1: a = "Always good to hear that band playing, they always bring me a lot of joy. We will be continuing with another one of their tunes called " + music.current_song + " Coming to you live, performed right here. Right now."; break;
		case 2: a = "How is everybody doing? The sun is shining and it's pretty hot out there, ouch, don't burn yourself. I think you will dig this next tune: " + music.current_song + ", it's straight fire."; break;
		case 3: a = "That was such a great song. Good to hear that band finally coming together again to create something that stunning. I am really looking forward to this next song, because it's from my favorite album that just came out 10 seconds ago. I hope you like this song just as I do. It's called, " + music.current_song + ". Here we go!"; break;
	}
	say(a);
}