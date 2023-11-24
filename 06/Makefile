CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wno-unused-variable
TEST_LIB=-lgtest_main -lgtest -lpthread

test_bin: tests.cpp
	$(CC) $(FLAGS) tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	rm -rf *.o test_bin