all:
	rm -f ./tests/run
	g++ -std=c++11 -Wall -Wextra -Werror -g -I. tests/*.c++ -o tests/run
	./tests/run > ./tests/log
