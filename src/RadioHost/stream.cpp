#include "stream.h"

void stream::start_stream() {
	streaming = true;
	switch(static_cast<hosts::name>(random(0,3))) {
		case hosts::name::Ryan_Elwick:		host = std::make_shared<RYAN_ELWICK>(RYAN_ELWICK());		break;
		case hosts::name::Tavish_DeGroot:	host = std::make_shared<TAVISH_DEGROOT>(TAVISH_DEGROOT());	break;
		case hosts::name::Michelle_Adams:	host = std::make_shared<MICHELLE_ADAMS>(MICHELLE_ADAMS());	break;
		case hosts::name::Steven_Hawking:	host = std::make_shared<STEVEN_HAWKING>(STEVEN_HAWKING());	break;
	}
	host->start_stream();
}

bool stream::callback() {
	return streaming;
}

void stream::stop_stream() {
	streaming = false;
}