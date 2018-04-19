#pragma once

#include "Host.h"

class TAVISH_DEGROOT : public Host {
	public:
		TAVISH_DEGROOT() { name = hosts::name::Tavish_DeGroot; vox = "awb"; }
	protected:
		void announce_start() override;
		void announce_next_song() override;
};

inline void TAVISH_DEGROOT::announce_start() {

}

inline void TAVISH_DEGROOT::announce_next_song() {

}