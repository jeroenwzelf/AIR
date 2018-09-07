# AIR - Artificial Intelligence Radio
A C++ project that composes music, performs it via MIDI, converts it to .wav and hosts continuous newly generated music.

## Running AIR
* Download the repository
* make and run *'./AIR'* in a terminal.
See the man page for any program options and other info

## Current state of development
Composer:
	[x] Generating scales
	[x] Generating chords
	[x] Generating progressions
	[ ] Generating songs
		[ ] Generating intro, verse, bridge, etc
		[ ] Neural network for creating nice songs
Performer:
	[x] Generating different parts for a song
	[ ] Picking random drum sequences from a file
	[x] Picking random MIDI instruments
	[ ] Neural network for picking nice sounding MIDI instruments
	[x] Convert parts into MIDI file
	[x] Convert MIDI file into .wav file
RadioHost:
	[x] Random hosts with different voices
	[x] Plays audio according to typical radio format (host announces song, song plays, host says something, next song plays, etc)
	[x] Manages the discord bot
Discord bot:
	[x] Basic voting system functionality
	[x] Saving votes from each user
	[x] Plays radio in voice channel (from a radio broadcast url)
