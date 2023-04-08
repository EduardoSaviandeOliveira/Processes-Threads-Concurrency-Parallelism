#include <array>
#include <ctime>
#include <cstdio>
#include <cstdlib>

// #include <omp.h>
#include <pthread.h>

#include "types.h"
#include "posix_utils.h"

using WorkerProc = void* (*)(void*); // Type alias for pthread procedures

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

namespace G { // Global var namespace
Display display;
constexpr usize BUF_SIZE = 4;
usize buf_index = 0;
std::array<f64, BUF_SIZE> weigh_in_buffer = {0};
}

// TODD: make thread safe
void push_weight(f64 w){
	if(G::buf_index >= G::BUF_SIZE){
		// clear buffer and update display
		G::buf_index = 0;
	}
	G::weigh_in_buffer[G::buf_index] = w;
	G::buf_index += 1;
}

struct Belt {
	uint delay;
	f64 weight;

	void run(){
		while(1){
			microsleep(delay * 1000000);
			printf("[%.1f/%d] pushed.\n", weight, delay);
			push_weight(weight);
		}
	}

	Belt(){}
	Belt(uint d, uint w) : delay(d), weight(w) {}
};

void* belt_run_wrapper(void* belt){
	Belt* b = (Belt*)belt;
	b->run();
	return NULL;
}

int main(){
	Belt* belt0 = new Belt(2, 5.0);
	Belt* belt1 = new Belt(1, 2.0);
	pthread_t belt0_thread, belt1_thread;
	if(pthread_create(&belt0_thread, NULL, belt_run_wrapper, belt0) != 0){
		std::fprintf(stderr, "Failed to create worker thread.\n");
		std::abort();
	}
	if(pthread_create(&belt1_thread, NULL, belt_run_wrapper, belt1) != 0){
		std::fprintf(stderr, "Failed to create worker thread.\n");
		std::abort();
	}

	while(1){
		microsleep(1);
	}

	delete belt0;
	delete belt1;
	return 0;
}

