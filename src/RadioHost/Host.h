#pragma once

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <unistd.h>

#include "note.h"
#include "music_handler.h"

namespace hosts {
	enum name { Ryan_Elwick, Tavish_DeGroot, Michelle_Adams, Steven_Hawking };
	inline std::string vox_of(const hosts::name &n) {
		switch (n) {
			case hosts::name::Ryan_Elwick:		return "rms";	break;
			case hosts::name::Tavish_DeGroot:	return "awb";	break;
			case hosts::name::Michelle_Adams:	return "slt";	break;
			case hosts::name::Steven_Hawking:	return "kal";	break;
		}
		return "";
	}
	inline std::string name_of(const hosts::name &n) {
		switch(n) {
			case hosts::name::Ryan_Elwick:		return "Ryan Elwick";		break;
			case hosts::name::Tavish_DeGroot:	return "Tavish De Groot";	break;
			case hosts::name::Michelle_Adams:	return "Michelle Adams";	break;
			case hosts::name::Steven_Hawking:	return "Steven Hawking";	break;
		}
		return "-";
	}
}

class Host {
	public:
		Host(hosts::name n, music_handler* m);
		hosts::name name;

		void announce_start();
		void announce_next_song(std::string songname);
	protected:
		std::string vox;
		music_handler* music;
		void say(std::string);
		void update();
};