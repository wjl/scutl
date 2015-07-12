all: default_report TAP_report

default_report:
	@echo -n "Testing default report..."
	@rm -f ./tests/reporters/default_run
	@g++ -std=c++14 -Wall -Wextra -Werror -g -I. $(sort $(wildcard tests/*.c++)) tests/reporters/default.c++ -o tests/reporters/default_run
	@./tests/reporters/default_run > ./tests/reporters/default.stdout.actual 2> ./tests/reporters/default.stderr.actual || true
	@diff -u tests/reporters/default.stdout.expected tests/reporters/default.stdout.actual
	@diff -u tests/reporters/default.stderr.expected tests/reporters/default.stderr.actual
	@echo "PASS."

TAP_report:
	@echo -n "Testing TAP report..."
	@rm -f ./tests/reporters/TAP_run
	@g++ -std=c++11 -Wall -Wextra -Werror -g -I. $(sort $(wildcard tests/*.c++)) tests/reporters/TAP.c++ -o tests/reporters/TAP_run
	@-./tests/reporters/TAP_run > ./tests/reporters/TAP.stdout.actual 2> ./tests/reporters/TAP.stderr.actual || true
	@diff -u tests/reporters/TAP.stdout.expected tests/reporters/TAP.stdout.actual
	@diff -u tests/reporters/TAP.stderr.expected tests/reporters/TAP.stderr.actual
	@echo "PASS."

new_expected:
	@echo "Saving actual output as new expected output."
	@cp tests/reporters/default.stdout.actual tests/reporters/default.stdout.expected
	@cp tests/reporters/default.stderr.actual tests/reporters/default.stderr.expected
	@cp tests/reporters/TAP.stdout.actual tests/reporters/TAP.stdout.expected
	@cp tests/reporters/TAP.stderr.actual tests/reporters/TAP.stderr.expected
