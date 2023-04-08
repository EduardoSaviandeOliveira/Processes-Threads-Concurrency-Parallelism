#include <iostream>
#include <array>
#include <ctime>

#include <omp.h>
#include <pthread.h>

#include "types.h"
#include "posix_utils.h"

using WorkerProc = void* (*)(void*); // Type alias for pthread procedures

constexpr usize BUF_SIZE = 500;
usize buf_index = 0;
std::array<f64, BUF_SIZE> weigh_in_buffer;

// TODD: make thread safe
void push_weight(f64 w){
	weigh_in_buffer[buf_index] = w;
	buf_index += 1;
}

struct Belt {
	uint delay;
	f64 weight;

	void run(){
		while(1){
			microsleep(delay * 1000000);
			push_weight(weight);
		}
	}
};

int main(){
	return 0;
}

