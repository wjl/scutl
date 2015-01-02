all: default_report TAP_report

default_report:
	rm -f ./tests/reporters/default_run
	g++ -std=c++11 -Wall -Wextra -Werror -g -I. $(sort $(wildcard tests/*.c++)) tests/reporters/default.c++ -o tests/reporters/default_run
	-./tests/reporters/default_run > ./tests/reporters/default.stdout 2> ./tests/reporters/default.stderr
	diff -u tests/reporters/default.stdout.gold tests/reporters/default.stdout
	diff -u tests/reporters/default.stderr.gold tests/reporters/default.stderr

TAP_report:
	rm -f ./tests/reporters/TAP_run
	g++ -std=c++11 -Wall -Wextra -Werror -g -I. $(sort $(wildcard tests/*.c++)) tests/reporters/TAP.c++ -o tests/reporters/TAP_run
	-./tests/reporters/TAP_run > ./tests/reporters/TAP.stdout 2> ./tests/reporters/TAP.stderr
	diff -u tests/reporters/TAP.stdout.gold tests/reporters/TAP.stdout
	diff -u tests/reporters/TAP.stderr.gold tests/reporters/TAP.stderr

new_gold:
	mv tests/reporters/default.stdout tests/reporters/default.stdout.gold
	mv tests/reporters/default.stderr tests/reporters/default.stderr.gold
	mv tests/reporters/TAP.stdout tests/reporters/TAP.stdout.gold
	mv tests/reporters/TAP.stderr tests/reporters/TAP.stderr.gold
