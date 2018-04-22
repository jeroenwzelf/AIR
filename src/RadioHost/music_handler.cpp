#include "music_handler.h"

typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the number of samples.
} header;

typedef struct header_file* header_p;

struct pactl_entry {
	pactl_entry(std::string e) {
		e += '\t';
		int i = 0;
		std::string current = "";
		for (auto& c : e) {
			if (c != '\n') {
				if (c != '\t') current += c;
				else {
					switch(i) {
						case 0: index = atoi(current.c_str()); break;
						case 1: sink = atoi(current.c_str()); break;
						case 2: client = atoi(current.c_str()); break;
						case 3: name = current; break;
						case 4: sample_specification = current; break;
					}
					current = ""; ++i;
				}
			}
		}
	}
	int index, sink, client;
	std::string name, sample_specification;
};

music_handler::music_handler() { time_remaining = 0; }

void music_handler::play_new_song() {
	// generate new song
	current_song = generateMIDI().generate(random(170, 270));

	// get length of song
	FILE * infile = fopen(("src/RadioHost/songs/" + current_song + ".wav").c_str(),"rb");		// Open wave file in read mode
	header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
	if (infile) {
		fread(meta, 1, sizeof(header), infile);
		time_remaining = meta->subchunk2_size / meta-> byte_rate;
	}

	printf("playing new song...\n");
	// playing song
	std::vector<std::string> arguments = {"aplay", ("src/RadioHost/songs/" + current_song + ".wav").c_str()};
	std::vector<char*> argv;
	for (const auto& arg : arguments)
	    argv.push_back((char*)arg.data());
	argv.push_back(nullptr);

	int pid = fork();
	if( pid < 0 ) throw;	// failed to fork
	else if (pid == 0) {
		execvp( argv[0], argv.data() );
		_Exit(EXIT_FAILURE);
	}
	nowplaying = true;
}

void music_handler::update() {
	if (time_remaining > 0) time_remaining--;
	else nowplaying = false;
}

void music_handler::set_song_volume(int volume) {
	if (nowplaying) {
		/* -- get all currently active sink inputs from pulseaudio driver -- */
		std::array<char, 128> buffer;
		std::vector<std::string> result;
	    std::shared_ptr<FILE> pipe(popen("pactl list sink-inputs short", "r"), pclose);
	    if (!pipe) throw std::runtime_error("popen() failed!");
	    while (!feof(pipe.get())) {
	        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
	        	result.push_back(std::string(buffer.data()));
	        }
	    }
	    std::vector<pactl_entry> entries;
	    for (auto& s : result) entries.push_back(pactl_entry(s));

	    int number = -1;
	    for (auto &entry : entries) 
	    	if (entry.sample_specification == "s16le 2ch 44100Hz") number = entry.index;
	    if (number >= 0) {
		    std::string cmd = "pactl set-sink-input-volume " + std::to_string(number) + ' ' + std::to_string(volume * 65536 / 100);
		    system(cmd.c_str());
	    }
	}
}

/*int music_handler::count_songs() {
    int len = 0;
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir("src/RadioHost/songs");
    if (pDir != NULL) while ((pDirent = readdir(pDir)) != NULL) len++;
    closedir(pDir);
    return len;
}*/