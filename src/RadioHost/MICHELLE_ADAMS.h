#pragma once

#include "Host.h"

class MICHELLE_ADAMS : public Host {
	public:
		MICHELLE_ADAMS() { name = hosts::name::Michelle_Adams; vox = "slt"; }
	protected:
		void announce_start() override;
		void announce_next_song() override;
};

inline void MICHELLE_ADAMS::announce_start() {

}

inline void MICHELLE_ADAMS::announce_next_song() {

}