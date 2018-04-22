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
	if (time_remaining > 0) {
		time_remaining--;
	}
	else {
		nowplaying = false;
	}
}

void music_handler::set_song_volume(int volume) {
	if (nowplaying) {
		std::array<char, 128> buffer;
	    std::string result;
	    std::shared_ptr<FILE> pipe(popen("pactl list sink-inputs | grep \"Sink Input\" ", "r"), pclose);
	    if (!pipe) throw std::runtime_error("popen() failed!");
	    while (!feof(pipe.get())) {
	        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
	            result += buffer.data();
	    }
	    result = result.substr(result.size() - 4);
	    std::string number = "";
	    for (auto &c : result) {
	    	if (isInteger(std::string(1,c))) number += c;
	    }
	    std::string cmd = "pactl set-sink-input-volume " + number + ' ' + std::to_string(volume * 65536 / 100);
	    system(cmd.c_str());
	}
}

void music_handler::remove_least_recent() {
	/*bool loaded = false;
	struct stat leastrecent_attrib;

	struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir("src/RadioHost/songs");
    if (pDir != NULL) {
    	while ((pDirent = readdir(pDir)) != NULL) {
    		if (pDirent->d_name[0] != '.') {
				struct stat attrib;
				stat(pDirent->d_name, &attrib);
				char date[10];

				if (!loaded) {
					//leastrecent_attrib = attrib;
					stat(pDirent->d_name, &leastrecent_attrib);
					loaded = true;
				}
				else {
					std::cout << "From " << pDirent->d_name << std::endl;
					printf("%.f\n", difftime(attrib.st_mtime, leastrecent_attrib.st_mtime));
					printf("%.f\n", difftime(leastrecent_attrib.st_mtime, attrib.st_mtime));
					//std::cout << attrib.st_mtime << std::endl << leastrecent_attrib.st_mtime << std::endl;
					strftime(date, 20, "%d-%m-%y", localtime(&(attrib.st_ctime)));
					std::cout << date << std::endl;
					strftime(date, 20, "%d-%m-%y", localtime(&(leastrecent_attrib.st_ctime)));
					std::cout << date << std::endl;
					//printf("%lld\n", leastrecent_attrib.st_mtime);
				}


				strftime(date, 20, "%d-%m-%y", localtime(&(attrib.st_ctime)));
				printf("The file %s was last modified at %s\n", pDirent->d_name, date);
				date[0] = 0;
			}
    	}
    }
    closedir(pDir);*/
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