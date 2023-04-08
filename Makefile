CC := clang++ -std=c++17
CFLAGS := -O2 -pipe -Wall -Wextra -fsanitize=address
LDFLAGS := -lasan -lpthread

.PHONY: clean foodsec

run: foodsec
	@./foodsec

foodsec: main.o
	$(CC) -o foodsec main.o $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -f foodsec *.o *.so *.a *.lib

