// Scutl -- Simple C++ Unit Testing Library
// ========================================
//
// Scutl is a unit testing library that makes it painless to add unit tests to
// your software with a minimum of hassle. The entire library implementation
// and all of its documentation is contained in this single header file. The
// only dependency is a C++ compiler.
//
// Quick Start Example
// -------------------
//
// This is all you really need to know to get started. Come back and read the
// documentation further only once you feel like it!
//
// In test.c++:
//
//     #define SCUTL_MAIN
//     #include "scutl.h++"
//
//     TEST(one_plus_one) {
//         int x = 1;
//         ASSERT(x == 1);
//     }
//
//     TEST(one_plus_one) {
//         int x = 1;
//         ASSERT((x+x) == 3);
//     }
//
// To build & run with GCC:
//
//     g++ test.c++ -o test && ./test
//
// Copyright & License Information
// -------------------------------
//
// Copyright © 2013 Wesley J. Landaker
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef SCUTL__header__
#define SCUTL__header__

// TODO: what exceptions required?
// TODO: what macros required?
// TODO: look at CheckMacros.h to implement ASSERT/REQUIRE
// TODO: look at TestMacros.h to implement SUITE
// TODO: look at Boost Test to see other types of checks / features
// TODO: look at crpcut for other features
// TODO: implement INFO stream like crpcut (probably just stdout)
// TODO: run tests in separate directory and process like crpcut
// TODO: handle uncaught exceptions (catch (...), void unexpected(), etc)
// TODO: handle exit/abort and assertions from <cassert>
// TODO: handle crashes
// TODO: handle signals -- see posix/SignalTranslator.h and crpcut
// TODO: handle infinite loops (timeouts)
// TODO: handle memory hogging
// TODO: handle forkbombs
// TODO: work with valgrind
// TODO: generic output API ("Test Reporter")
// TODO: report start of test
// TODO: report each check (pass/fail/results)
// TODO: report stream activity (stdout, stderr)
// TODO: report end of test (elapsed time)
// TODO: report summary (tests: pass/total, checks: pass/total, elapsed time)
// TODO: TAP output for use with prove
// TODO: ASSERT/REQUIRE use class members or global variables?
// TODO: automatic timing
// TODO: handle expected failures
// TODO: handle test fixtures
// TODO: run or exclude specific tests or suites

#include <exception>
#include <list>
#include <string>

// If the main implementation is requested, we need to add the library
// implementation as well.
#ifdef SCUTL_MAIN
#ifndef SCUTL_LIBRARY
#define SCUTL_LIBRARY
#endif
#endif

// Helper macros for concatenation & stringification.
#define SCUTL_DETAIL_CONCAT_RAW(a, b) a ## b
#define SCUTL_DETAIL_CONCAT(a, b) SCUTL_DETAIL_CONCAT_RAW(a, b)

// Define a test with the given name and provided body.
//
//     TEST(name) { body }
#ifdef TEST
#error "Scutl wants to provide the TEST macro, but it's already defined!"
#endif
#define TEST(name) TEST_METHOD(::scutl::detail::Empty_Fixture, name)

// Define a test method on an instance of the given fixture class with the
// given name and provided body.
//
//     TEST_METHOD(fixture, name) { body }
//
#ifdef TEST_METHOD
#error "Scutl wants to provide the TEST_METHOD macro, but it's already defined!"
#endif
#define TEST_METHOD(fixture, name)\
\
	/* Enter a uniquified anonymous namespace to avoid conflicts */\
	namespace {\
	namespace SCUTL_DETAIL_CONCAT(scutl_detail_line_,__LINE__) {\
\
	/* Define our Test class */\
	struct Test : ::scutl::detail::Test {\
		Test() {\
			na##me = #name;\
			file = __FILE__;\
			line = __LINE__;\
		}\
		virtual void operator()() const override { function(); };\
		/* The function is implemented in an inner class inheriting from the\
		 * provided fixture class to ensure that the user's test function\
		 * does not have a polluted namespace. */\
		struct Function : fixture {\
			void operator()();\
		};\
		mutable Function function;\
	};\
\
	/* Create an instance of the test */\
	Test test;\
\
	/* Close the namespaces we opened */\
	}}\
\
	/* Define our test function's body */\
	void SCUTL_DETAIL_CONCAT(scutl_detail_line_,__LINE__)\
		::Test::Function::operator()()

// Helper macro for impementing assertions.
#define SCUTL_DETAIL_ASSERT(expression, assertion, required)\
\
	/* Wrap in do/while to give an anonymous scope */\
	do {\
		/* Collect the results into an Results object. */\
		::scutl::detail::Results results (\
			assertion #expression,\
			__FILE__,\
			__LINE__,\
			(expression)\
		);\
\
		/* Report results and throw if necessary. */\
		::scutl::detail::results_list.push_back(results);\
		if (required && !results.pass) throw results;\
	} while(0)

// Expect that the given expression is true.
//
//     TEST(name) {
//         ASSERT(expression);
//     }
//
#ifdef ASSERT
#error "Scutl wants to provide the ASSERT macro, but it's already defined!"
#endif
#define ASSERT(expression) SCUTL_DETAIL_ASSERT((expression), "ASSERT", false)

// Require that the given expression is true; otherwise abort the test.
//
//     TEST(name) {
//         REQUIRE(expression);
//     }
//
#ifdef REQUIRE
#error "Scutl wants to provide the REQUIRE macro, but it's already defined!"
#endif
#define REQUIRE(expression) SCUTL_DETAIL_ASSERT((expression), "REQUIRE", true)

// This namespace is for implementation details.
namespace scutl { namespace detail {

	// Define a Test class to encapsulate our tests, and a Test_List
	// class to hold them as they are registered.
	struct Test;
	typedef std::list<Test *> Test_List;

	struct Test {
		Test() { list().push_back(this); }
		virtual ~Test() {}
		std::string name;
		std::string file;
		size_t line;
		virtual void operator()() const = 0;
		static Test_List &list();
	};

	// Define an empty fixture class that's used when nothing is provided
	// by the user.
	struct Empty_Fixture {};

	// Holds the results of an individual test assertion.
	struct Results {

		Results(
			std::string expression,
			std::string file,
			size_t line,
			bool pass
		) : expression(expression), file(file), line(line), pass(pass) {}

		Results(const Test &test, std::string expression)
			: expression(expression), file(test.file), line(test.line), pass(false) {}

		std::string expression;
		std::string file;
		size_t line;
		bool pass;
	};

	// List holding all the individual results of the current test.
	typedef std::list<Results> Results_List;
	extern Results_List results_list;

}}

#ifdef SCUTL_LIBRARY
#include <iostream>
namespace scutl { namespace detail {

	// Implement our test case list. We use a function to return a single
	// instance instead of a straight global variable to avoid static
	// initialization ordering issues.
	Test_List &Test::list() {
		static Test_List list;
		return list;
	}

	// Implement our global test results list.
	Results_List results_list;

	// Process the results list after a test has completed. Returns the number
	// of failing assertions.
	bool process_results_list(const Test &test) {
		// Give details for failing tests
		// FIXME: this just goes to stdout for now, but normally we'll
		// FIXME: go through the a test reporter interface
		size_t passed = 0;
		size_t total  = 0;
		for (
			Results_List::iterator results_iterator = results_list.begin();
			results_iterator != results_list.end();
			++results_iterator
		) {
			Results &results = *results_iterator;
			++total;
			if (!results.pass) {
				std::cout
					<< results.file << ":"
					<< results.line << ":"
					<< test.name << ":"
					<< results.expression << '\n'
				;
			} else {
				++passed;
			}
		}
		// Give summary
		// FIXME: this just goes to stdout for now, but normally we'll
		// FIXME: go through the a test reporter interface
		std::cout
			<< test.file << ":"
			<< test.line << ":"
			<< test.name << ":"
			<< passed << " of "
			<< total << " tests passed\n"
		;

		// Return the number of failed assertions.
		return total-passed;
	}

	// Helper function to run a single test. Returns the total number of
	// failing assertions.
	size_t run_test(const Test &test) {
		try {
			// Clear the global results
			results_list.clear();

			// Run the test
			test();

		// Catch exceptions that may be thrown by the test we're running, and
		// turn them into new results if necessary.
		} catch (const Results &) {
			// Results received as an exception have already been added to
			// the results list, so don't need additional handling.
			// FIXME: but we need to note that the test was aborted so that
			// FIXME: we don't give the wrong impression about the number of
			// FIXME: total tests that exist in our summary.
		} catch (const std::exception &e) {
			results_list.push_back(Results(test, "caught std::exception"));
		} catch (const std::string &e) {
			results_list.push_back(Results(test, "caught std::string"));
		} catch (...) {
			results_list.push_back(Results(test, "caught unknown exception"));
		}

		// After running a test, process all of it's results.
		return process_results_list(test);
	}
}}

namespace scutl {

	// The scutl entry point, which can be called by the user when ready to
	// run all the registered tests.
	int main() {
		scutl::detail::Test_List &test_list = scutl::detail::Test::list();
		size_t passed = 0;
		size_t total = 0;
		for (
			scutl::detail::Test_List::iterator test_iterator = test_list.begin();
			test_iterator != test_list.end();
			++test_iterator
		) {
			scutl::detail::Test &test = **test_iterator;
			++total;
			passed += scutl::detail::run_test(test);
		}

		// Give summary of total tests passed or failed.
		// FIXME: this just goes to stdout for now, but normally we'll
		// FIXME: go through the a test reporter interface
		std::cout
			<< passed << " of "
			<< total << " tests passed\n"
		;

		// Our return code is the number of failing tests.
		return total-passed;
	}
}
#endif

#ifdef SCUTL_MAIN
int main() {
	return scutl::main();
}
#endif

#endif
