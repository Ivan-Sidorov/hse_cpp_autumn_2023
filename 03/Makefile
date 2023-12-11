CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wno-unused-variable
TEST_LIB=-lgtest_main -lgtest -lpthread

all: matrix test_bin

matrix: Matrix.cpp Matrix.hpp
	$(CC) $(FLAGS) -c Matrix.cpp

test_bin: Matrix.o tests.cpp
	$(CC) $(FLAGS) Matrix.o tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	rm -rf *.o test_bin