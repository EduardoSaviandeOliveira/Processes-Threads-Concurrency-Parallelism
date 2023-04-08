#include <array>
#include <ctime>
#include <cstdio>
#include <cstdlib>

// #include <omp.h>

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
std::array<f64, BUF_SIZE> weigh_in_buffer = {0};
usize buf_index = 0;
pthread_mutex_t buf_lock = PTHREAD_MUTEX_INITIALIZER;
}

void push_weight(f64 w){
	pthread_mutex_lock(&G::buf_lock);
	if(G::buf_index >= G::BUF_SIZE){ // Clear buffer and update display
		f64 acc = 0;
		// TODO: Vectorize with OpenMP
		for(const auto& n : G::weigh_in_buffer){ acc += n; }
		G::display.val += acc;

		G::buf_index = 0;
		// TODO: Vectorize with OpenMP
		for(auto& n : G::weigh_in_buffer){ n = 0; }
	}
	G::weigh_in_buffer[G::buf_index] = w;
	G::buf_index += 1;
	pthread_mutex_unlock(&G::buf_lock);
}

struct Belt {
	usize delay; // Push delay, in ms
	f64 weight;

	void run(){
		while(1){
			microsleep(delay * 1000);
			std::fprintf(stderr,"belt: [%.1f | %zu] pushed.\n", weight, delay);
			push_weight(weight);
		}
	}

	Belt(){}
	Belt(uint d, f64 w) : delay(d), weight(w) {}
};

void* belt_run_wrapper(void* belt){
	Belt* b = (Belt*)belt;
	b->run();
	return NULL;
}

void* display_run_wrapper(void* display){
	Display* d = (Display*)display;
	d->run();
	return NULL;
}

int main(){
	Belt* belt0 = new Belt(2000, 5.0);
	Belt* belt1 = new Belt(1000, 2.0);
	pthread_t belt0_thread, belt1_thread, display_thread;

	if(pthread_create(&display_thread, NULL, display_run_wrapper, &G::display) != 0){
		std::fprintf(stderr, "Failed to create worker thread.\n");
		std::abort();
	}
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

