#pragma once

#include <algorithm>

#include "note.h"
#include "Host.h"

class stream {
	public:
		void start_stream();
		bool callback();
		void stop_stream();
	private:
		Host* host;
		bool streaming;
};