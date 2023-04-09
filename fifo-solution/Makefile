CC := clang++ -std=c++17
CFLAGS := -O2 -pipe -Wall -Wextra
LDFLAGS := -lpthread

.PHONY: clean foodsec

all: belt

belt: belt.o
	$(CC) -o belt belt.o $(LDFLAGS)

belt.o: belt.cpp
	$(CC) $(CFLAGS) -c belt.cpp -o belt.o

clean:
	rm -f belt display foodsec *.o *.so *.a *.lib

