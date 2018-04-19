#pragma once

#include <algorithm>

#include "note.h"
#include "Host.h"
#include "RYAN_ELWICK.h"
#include "TAVISH_DEGROOT.h"
#include "MICHELLE_ADAMS.h"
#include "STEVEN_HAWKING.h"

class stream {
	public:
		void start_stream();
		bool callback();
		void stop_stream();
	private:
		std::shared_ptr<Host> host;
		bool streaming;
};