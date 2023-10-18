CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wno-unused-variable
TEST_LIB=-lgtest_main -lgtest -lpthread

all: big_int test_bin

big_int: BigInt.cpp BigInt.hpp
	$(CC) $(FLAGS) -c BigInt.cpp

test_bin: BigInt.o tests.cpp
	$(CC) $(FLAGS) BigInt.o tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	rm -rf *.o test_bin