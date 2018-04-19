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

}

inline void STEVEN_HAWKING::announce_next_song() {

}