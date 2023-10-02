CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wno-unused-variable
TEST_LIB=-lgtest_main -lgtest -lpthread

all: parser test_bin

parser: TokenParser.cpp TokenParser.hpp
	$(CC) $(FLAGS) -c TokenParser.cpp

test_bin: TokenParser.o tests.cpp
	$(CC) $(FLAGS) TokenParser.o tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	rm -rf *.o test_bin