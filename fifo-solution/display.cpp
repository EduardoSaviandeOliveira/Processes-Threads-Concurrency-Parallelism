#include <array>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "types.h"
#include "posix_utils.h"

f64 read_f64(int fd){
	f64 n;
	byte* p = (byte*) &n;
	read(fd, p, sizeof(f64));
	return n;
}

void show(f64 val){
	std::printf("[ %.2f ]\n", val);
}

int main(){
	mkfifo("belt0.fifo", 0666);
	// mkfifo("belt1.fifo", 0666);
	for(usize i = 0; i < 10; i += 1){
		int fd0 = open("belt0.fifo", O_RDONLY);
		// int fd1 = open("belt1.fifo", O_RDONLY);
		show(read_f64(fd0));
		// show(read_f64(fd1));
		close(fd0);
		// close(fd1);
		microsleep(2000);
	}
	unlink("belt0.fifo");
	// unlink("belt1.fifo");
	return 0;
}
