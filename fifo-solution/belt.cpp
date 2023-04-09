#include <array>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>

#include "types.h"
#include "posix_utils.h"

struct Display {
	f64 val;

	void show(){
		std::printf("[ %.2f ]\n", val);
	}

	void run(){
		while(1){
			show();
			microsleep(2 * 1000000);
		}
	}
};

struct Belt {
	usize delay; // Push delay, in ms
	f64   weight;
	const char* fifo_path;

	void run(){
		while(1){
			int fd = open(fifo_path, O_WRONLY);
			if(fd < 0){
				std::fprintf(stderr, "Failed to open file: %s\n", fifo_path);
				return;
			}
			printf("%.2f\n", weight);
			close(fd);

			microsleep(delay * 1000);
		}
	}

	Belt(){}
	Belt(uint d, f64 w, const char* p) : delay(d), weight(w), fifo_path(p) {}
};

int main(int argc, const char** argv){
	if(argc < 4){
		std::fprintf(stderr, "Usage: belt [DELAY] [VAL] [FIFO]\n");
		return 1;
	}
	uint d = atoi(argv[1]);
	f64 v = atof(argv[2]);
	const char* file = argv[3];

	Belt b(d, v, file);
	b.run();
	return 0;
}

