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

music_handler::music_handler() {
	time_remaining = 0;
}

inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

void music_handler::play_new_song() {
	if (count_songs() > MAX_SONG_HISTORY) remove_least_recent();

	// generate new song
	current_song = generateMIDI().generate(random(170, 270));

	// get length of song
	FILE * infile = fopen(("src/RadioHost/songs/" + current_song + ".wav").c_str(),"rb");		// Open wave file in read mode
	header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
	if (infile) {
		fread(meta, 1, sizeof(header), infile);
		time_remaining = meta->subchunk2_size / meta-> byte_rate;
	}

	// playing song
	std::cout << "playing .wav file..." << std::endl;
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

	std::cout << "playing started succesfully. " << time_remaining << " more seconds to go." << std::endl;
	nowplaying = true;
}

void music_handler::update() {
	if (time_remaining > 0) {
		std::cout << "Song will be playing for " << time_remaining << " more seconds." << std::endl;
		time_remaining--;
	}
	else {
		nowplaying = false;
	}
}

void music_handler::remove_least_recent() {

}

int music_handler::count_songs() {
    int len = 0;
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir("src/RadioHost/songs");
    if (pDir != NULL) while ((pDirent = readdir(pDir)) != NULL) len++;
    closedir(pDir);
    return len;
}

int music_handler::song_length(std::string name) {
	std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen( ("mediainfo src/RadioHost/songs/" + name + ".wav | grep Duration").c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    result = result.substr(98, 4);
    std::string number = "";
    for (auto &c : result) {
    	if (isInteger(std::string(1,c))) number += c;
    }
    std::cout << "should be: " << number << std::endl;
    return atoi(number.c_str());
}

music_handler::~music_handler() { kill(pid_music, SIGKILL); }