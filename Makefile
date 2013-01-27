all:
	rm -f ./tests/run
	g++ -std=c++11 -Wall -Wextra -Werror -g -I. tests/*.c++ -o tests/run
	./tests/run > ./tests/log.stdout 2> ./tests/log.stderr
	diff -u tests/log.stdout.gold tests/log.stdout
	diff -u tests/log.stdout.gold tests/log.stdout
	diff -u tests/log.stderr.gold tests/log.stderr
	diff -u tests/log.stderr.gold tests/log.stderr
