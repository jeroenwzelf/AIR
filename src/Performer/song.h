#pragma once

#include "note.h"
#include "scale.h"
#include "chord.h"
#include "progression.h"

struct Note {
	Note() {};
	Note(int n) : note(n), duration(1), patch_bank(0), vol(0) {}
	Note(int n, unsigned d, int v) : note(n), duration(d), vol(v) {}
    Note(int n, unsigned d, unsigned p, int v)
        : note(n), duration(d), patch_bank(p), vol(v) {}
    int note;
    unsigned duration;
    unsigned patch_bank;
    int vol;
};

struct song {
	song(unsigned b);
	void generate_layout();
	~song();

	progression chordprogression;
	chord** layout;
	unsigned beats;
};