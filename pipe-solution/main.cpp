#include <array>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "types.h"
#include "posix_utils.h"

static void serialize_f64(byte* buf, f64 n){
	const byte* p = (const byte*)&n;
	for(usize i = 0; i < sizeof(f64); i += 1){
		buf[i] = p[i];
	}
}

f64 read_f64(int fd){
	byte buf[sizeof(f64) + 1] = {0};
	f64 n;
	read(fd, buf, sizeof(f64));
	n = * ((f64*)buf);
	return n;
}

namespace G {
const char* belt0_path = "/tmp/belt0.fifo";
const char* belt1_path = "/tmp/belt1.fifo";
constexpr usize BUF_SIZE = 4;
std::array<f64, BUF_SIZE> weigh_in_buf = {0};
usize buf_index = 0;
}

struct Display {
	f64 val;

	void show(){
		std::printf("[ %.2f ]\n", val);
	}

	void run(){
		while(1){
			int fd0 = open(G::belt0_path, O_RDONLY);
			// int fd1 = open(G::belt1_path, O_RDONLY);
			if(fd0 < 0 /*|| (fd1 < 0)*/){
				std::fprintf(stderr, "Failed to open file.\n");
				std::abort();
			}
			if(G::buf_index >= G::BUF_SIZE){
				f64 acc = 0;
				for(const auto& n : G::weigh_in_buf){ acc += n; }
				val = acc;
				for(auto& n : G::weigh_in_buf){ n = 0; }
				G::buf_index = 0;
			}
			auto n = read_f64(fd0);
			printf("[fd0] %f\n", n);
			G::weigh_in_buf[G::buf_index] = n;
			G::buf_index += 1;
			// G::weigh_in_buf[G::buf_index] = read_f64(fd1);
			// G::buf_index += 1;
			show();
			close(fd0);
			// close(fd1);
			microsleep(2 * 1000000);
		}
	}
};

struct Belt {
	usize delay; // Push delay, in ms
	f64 weight;
	const char* pipe_path;

	void run(){
		byte buf[sizeof(f64) + 1] = {0};
		while(1){
			serialize_f64(buf, weight);
			int fd = open(pipe_path, O_WRONLY);
			if(fd < 0){
				std::fprintf(stderr, "Failed to open file.\n");
				std::abort();
			}
			write(fd, buf, sizeof(f64));
			close(fd);
			std::fprintf(stderr,"belt: [%.1f | %zu] pushed.\n", weight, delay);
			// push_weight(weight);
			microsleep(delay * 1000);
		}
	}

	Belt(){}
	Belt(uint d, f64 w, const char* p) : delay(d), weight(w), pipe_path(p) {
		mkfifo(pipe_path, S_IRWXU);
	}

	~Belt(){
		unlink(pipe_path);
	}
};

int main(){
	Belt b0(200, 5.0, G::belt0_path);
	Belt b1(100, 2.0, G::belt1_path);
	Display d = {0};
	pid_t pid = fork();
	if(pid < 0){ return 1; }
	if(pid == 0){
		// pid_t pid1 = fork();
		// if(pid1 == 0){
		// 	b1.run();
		// }
		// else {
		// 	b0.run();
		// }
		b0.run();
	}
	else {
		d.run();
	}
	return 0;
}
