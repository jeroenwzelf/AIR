#pragma once

#include "Host.h"

class RYAN_ELWICK : public Host {
	public:
		RYAN_ELWICK() { name = hosts::name::Ryan_Elwick; vox = "rms"; }
	protected:
		void announce_start() override;
		void announce_next_song() override;
};

inline void RYAN_ELWICK::announce_start() {

}

inline void RYAN_ELWICK::announce_next_song() {

}