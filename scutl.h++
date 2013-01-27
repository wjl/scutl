// Scutl -- Simple C++ Unit Testing Library
// ========================================
// Version 0.0.0
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
//         EXPECT(x == 1);
//     }
//
//     TEST(one_plus_one) {
//         int x = 1;
//         EXPECT((x+x) == 3);
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

// TODO: handle uncatchable exceptions (e.g. unexpected()? what else is helpful?)
// TODO: work with valgrind
// TODO: prefix output "SCUTL: " to distinguish from other stream activity (stdout, stderr)
// TODO: generic output API refinement
// TODO: TAP output reporter for use with prove
// TODO: auto timing & summarizing in reporter base class
// TODO: assertions for expecting/not-expecting exceptions

// Required standard library headers
#include <iostream>
#include <list>
#include <string>

// Helper macros for concatenation & stringification.
#define SCUTL_DETAIL_CONCAT_RAW(a, b) a ## b
#define SCUTL_DETAIL_CONCAT(a, b) SCUTL_DETAIL_CONCAT_RAW(a, b)

// Define a test with the given name and provided body.
//
//     TEST(name) { body }
//
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
	/* The function is implemented in an class inheriting from the provided\
	 * fixture class. This is done separately from the Test object to ensure\
	 * that the user's test function does not have a polluted namespace. */\
	struct Function : fixture {\
		void operator()();\
	};\
\
	/* Define our Test class */\
	struct Test : ::scutl::detail::Test {\
		Test() {\
			info.na##me = #name;\
			info.file = __FILE__;\
			info.line = __LINE__;\
		}\
		virtual void operator()() {\
			function();\
		};\
		Function function;\
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
		::Function::operator()()


// Helper macro for impementing assertions.
#define SCUTL_DETAIL_ASSERT(expression, assertion, required)\
\
	/* Wrap in do/while to give an anonymous scope. */\
	do {\
		/* Until we have evaluated the expression, we have to use fully\
		 * qualified variable names */\
\
		/* Collect information about the assertion. */\
		::scutl::Assertion_Info scutl_detail_info;\
		scutl_detail_info.requ##ired = required;\
		scutl_detail_info.expre##ssion = assertion "(" #expression ")";\
		scutl_detail_info.file = __FILE__;\
		scutl_detail_info.line = __LINE__;\
		::scutl::detail::global.reporter->assertion_started(scutl_detail_info);\
\
		/* Test the expression */\
		bool scutl_detail_pass = (expression);\
\
		/* Report results and throw if necessary. */\
		::scutl::detail::global.reporter->assertion_complete(\
			scutl_detail_info,\
			scutl_detail_pass\
		);\
		if (required && !scutl_detail_pass) throw scutl_detail_info;\
	} while(0)

// Expect that the given expression is true.
//
//     TEST(name) {
//         EXPECT(expression);
//     }
//
#ifdef EXPECT
#error "Scutl wants to provide the EXPECT macro, but it's already defined!"
#endif
#define EXPECT(expression) SCUTL_DETAIL_ASSERT(expression, "EXPECT", false)

// Require that the given expression is true; otherwise abort the test.
//
//     TEST(name) {
//         REQUIRE(expression);
//     }
//
#ifdef REQUIRE
#error "Scutl wants to provide the REQUIRE macro, but it's already defined!"
#endif
#define REQUIRE(expression) SCUTL_DETAIL_ASSERT(expression, "REQUIRE", true)

// Functions & classes in the public API
namespace scutl {

	// Information about a test
	struct Test_Info {
		std::string name;
		std::string file;
		size_t      line;
	};

	// Information about an assertion
	struct Assertion_Info {
		bool        required;
		std::string expression;
		std::string file;
		size_t      line;
	};

	// Abstract interface for reporters of test results
	struct Reporter {
		Reporter() {}
		virtual ~Reporter() {}
		virtual void test_started       (const Test_Info &) = 0;
		virtual void test_complete      (const Test_Info &) = 0;
		virtual void assertion_started  (const Assertion_Info &) = 0;
		virtual void assertion_complete (const Assertion_Info &, bool passed) = 0;
	};

	// Simple reporter writes test info to stdout and errors/summary to stderr
	struct Simple_Reporter : Reporter {
		Simple_Reporter();
		virtual ~Simple_Reporter();
		virtual void test_started       (const Test_Info &);
		virtual void test_complete      (const Test_Info &);
		virtual void assertion_started  (const Assertion_Info &);
		virtual void assertion_complete (const Assertion_Info &, bool passed);

		private:
		size_t total_tests;
		size_t passed_tests;
		size_t passed_assertions_this_test;
		size_t total_assertions_this_test;
		size_t total_assertions;
		size_t passed_assertions;
	};

};

// This namespace is for implementation details.
namespace scutl { namespace detail {

	// Base class to encapsulate tests. They are auto-registered in a global
	// list upon construction.
	struct Test {
		Test() { list().push_back(this); }
		virtual ~Test() {}
		virtual void operator()() = 0;

		// Test information
		Test_Info info;

		// Global test list
		typedef std::list<Test *> List;
		static List &list();
	};

	// Define an empty fixture class that's used when nothing is provided
	// by the user.
	struct Empty_Fixture {};

	// Global pointers set by the test runner and use by assertions
	struct Global {
		Test     *test;
		Reporter *reporter;
	};
	extern Global global;

}}

// If the main implementation is requested, we need to add the library
// implementation as well.
#ifdef SCUTL_MAIN
#ifndef SCUTL_LIBRARY
#define SCUTL_LIBRARY
#endif
#endif

// Implement the scutl library when requested
#ifdef SCUTL_LIBRARY
namespace scutl { namespace detail {

	// Implement our list of tests. We use a function to return a single
	// instance instead of a straight global variable to avoid static-
	// initialization ordering issues.
	Test::List &Test::list() {
		static Test::List list;
		return list;
	}

	// Implement our global structure used to by the runner and assertions.
	Global global;

	// Helper function to run a single test with a given reporter.
	void run_test(Test &test, Reporter &reporter) {

		// Report the test to the reporter.
		reporter.test_started(test.info);

		// Set the global test and reporter pointers for use by assertions.
		global.test     = &test;
		global.reporter = &reporter;

		// Run the test inside a try block. If we catch any exceptions,
		// report them as implicit failed assertions.
		try {
			test();
		} catch (const Assertion_Info &) {
			// If an Assertion_Info is received as an exception, we know it
			// has already been reported before being thrown, so additional
			// handling is unnecessary here.
		} catch (const std::exception &e) {
			Assertion_Info info;
			info.required = true;
			info.expression = std::string("unexpected exception: ") + e.what();
			info.file = test.info.file;
			info.line = test.info.line;
			reporter.assertion_started(info);
			reporter.assertion_complete(info, false);
		} catch (...) {
			Assertion_Info info;
			info.required = true;
			info.expression = "unknown exception";
			info.file = test.info.file;
			info.line = test.info.line;
			reporter.assertion_started(info);
			reporter.assertion_complete(info, false);
		}

		// Report the test completion
		reporter.test_complete(test.info);
	}
}}

namespace scutl {

	// The scutl entry point, which can be called by the user when ready to
	// run all the registered tests against the given reporter.
	void run(Reporter &reporter) {
		scutl::detail::Test::List &test_list = scutl::detail::Test::list();
		for (
			scutl::detail::Test::List::iterator test_iterator = test_list.begin();
			test_iterator != test_list.end();
			++test_iterator
		) {
			scutl::detail::Test &test = **test_iterator;
			scutl::detail::run_test(test, reporter);
		}
	}

	Simple_Reporter::Simple_Reporter() {
		std::cout << "Starting test suite ...\n";
		passed_tests = 0;
		total_tests = 0;
		passed_assertions = 0;
		total_assertions = 0;
		passed_assertions_this_test = 0;
		total_assertions_this_test = 0;
	}

	Simple_Reporter::~Simple_Reporter() {
		std::cout << "Test suite complete\n";

		std::cerr
			<< passed_tests << " of "
			<< total_tests << " passed tests.\n"
		;
		if (passed_tests == total_tests) {
			std::cerr << "ALL TESTS PASSED!\n";
		} else {
			std::cerr
				<< (total_tests-passed_tests)
				<< " TESTS FAILED!\n";
		}

		std::cerr
			<< passed_assertions << " of "
			<< total_assertions << " passed assertions.\n"
		;
			if (passed_assertions == total_assertions) {
				std::cerr << "ALL ASSERTIONS PASSED!\n";
			} else {
			std::cerr
				<< (total_assertions-passed_assertions)
				<< " ASSERTIONS FAILED!\n";
			}

	}

	void Simple_Reporter::test_started (const Test_Info &info) {
		std::cout
			<< info.file << ":"
			<< info.line << ":"
			<< info.name << ": starting\n";
		;
		++total_tests;
		passed_assertions_this_test = 0;
		total_assertions_this_test = 0;
	}

	void Simple_Reporter::test_complete (const Test_Info &info) {
		std::cout
			<< info.file << ":"
			<< info.line << ":"
			<< info.name << ": complete\n"
		;
		if (
			(passed_assertions_this_test > 0) &&
			(passed_assertions_this_test == total_assertions_this_test)
		) {
			++passed_tests;
			passed_assertions_this_test = 0;
			total_assertions_this_test = 0;
		}
	}

	void Simple_Reporter::assertion_started (const Assertion_Info &info) {
		std::cout
			<< info.file << ":"
			<< info.line << ":"
			<< info.expression << ": starting\n"
		;
		++total_assertions_this_test;
		++total_assertions;
	}

	void Simple_Reporter::assertion_complete (const Assertion_Info &info, bool passed) {
		if (passed) {
			++passed_assertions_this_test;
			++passed_assertions;
		} else {
			std::cerr
				<< info.file << ":"
				<< info.line << ":"
				<< info.expression << ": FAIL\n"
			;
		}
	}

}
#endif

// Implement the main function when requested.
#ifdef SCUTL_MAIN
int main() {
	scutl::Simple_Reporter reporter;
	scutl::run(reporter);
	return 0;
}
#endif

#endif
