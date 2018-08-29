#include "stream.h"

void stream::start_stream() {
	streaming = true;
	host = new Host(static_cast<hosts::name>(random(0,3)));
	host->start_stream();
}

bool stream::callback() {
	return streaming;
}

void stream::stop_stream() {
	streaming = false;
}