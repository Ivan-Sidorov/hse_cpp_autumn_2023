CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wno-unused-variable
TEST_LIB=-lgtest_main -lgtest -lpthread

all: allocator test_bin

allocator: Allocator.cpp Allocator.hpp
	$(CC) $(FLAGS) -c Allocator.cpp

test_bin: Allocator.o tests.cpp
	$(CC) $(FLAGS) Allocator.o tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	rm -rf *.o test_bin