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
// TODO: TAP output reporter for use with prove
// TODO: assertions for expecting/not-expecting exceptions

// Required standard library headers
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
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
			info.na##me  = #name;\
			info.file    = __FILE__;\
			info.line    = __LINE__;\
			info.passed  = true;\
			info.aborted = true;\
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
	/* Wrap in do/while to give a statement-like interface. */\
	do {\
		/* If there was failure report results and throw if necessary. */\
		if (!(expression)) {\
			/* Collect information about the assertion. */\
			::scutl::Error_Info error_info;\
			error_info.expre##ssion = assertion "(" #expression ")";\
			error_info.file = __FILE__;\
			error_info.line = __LINE__;\
\
			/* Mark current test as not passed */\
			::scutl::detail::global.test->info.passed = false;\
\
			/* Report results and throw if necessary. */\
			::scutl::detail::global.reporter->report_test_error(\
				::scutl::detail::global.test->info,\
				error_info\
			);\
			if (required) {\
				::scutl::detail::global.test->info.aborted = true;\
				throw error_info;\
			}\
		}\
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
		bool        passed;
		bool        aborted;
	};

	// Information about a failing assertion
	struct Error_Info {
		std::string expression;
		std::string file;
		size_t      line;
	};

	// Statistics collected during test runnning
	struct Test_Statistics {
		size_t count;
		size_t started;
		size_t complete;
		size_t passed;
		size_t failed;
		size_t aborted;
	};

	// Interface for reporters of test results
	struct Reporter {
		Reporter() {}
		virtual ~Reporter() {}
		virtual void report_test_count   (size_t) = 0;
		virtual void report_test_started (const Test_Info &) = 0;
		virtual void report_test_complete(const Test_Info &) = 0;
		virtual void report_test_error   (const Test_Info &, const Error_Info &) = 0;
		virtual void report_test_summary (const Test_Statistics &) = 0;
	};

	// Simple reporter writes complete test log to stdout and errors/summary to stderr
	struct Simple_Reporter : Reporter {
		Simple_Reporter();
		virtual ~Simple_Reporter();
		virtual void report_test_count   (size_t);
		virtual void report_test_started (const Test_Info &);
		virtual void report_test_complete(const Test_Info &);
		virtual void report_test_error   (const Test_Info &, const Error_Info &);
		virtual void report_test_summary (const Test_Statistics &);
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
	void run_test(Test &test, Reporter &reporter, Test_Statistics &statistics) {

		// Set the global test and reporter pointers for use by assertions.
		global.test     = &test;
		global.reporter = &reporter;

		// Report the test to the reporter.
		reporter.report_test_started(test.info);
		++statistics.started;

		// Run the test inside a try block. If we catch any exceptions,
		// report them as implicit failed assertions.
		try {
			test();
			// By default test assume they will be aborted. If we don't get
			// an exception, then we can be assured that didn't happen.
			test.info.aborted = false;
		} catch (const Error_Info &) {
			// If an Error_Info is received as an exception, we know it
			// has already been reported before being thrown, so additional
			// handling is unnecessary here.
		} catch (const std::exception &e) {
			Error_Info error_info;
			error_info.expression = std::string("unexpected exception: ") + e.what();
			error_info.file = test.info.file;
			error_info.line = test.info.line;
			reporter.report_test_error(test.info, error_info);
		} catch (...) {
			Error_Info error_info;
			error_info.expression = "unknown exception";
			error_info.file = test.info.file;
			error_info.line = test.info.line;
			reporter.report_test_error(test.info, error_info);
		}

		// Report the test completion
		reporter.report_test_complete(test.info);
		++statistics.complete;
		if (test.info.passed)  ++statistics.passed; else ++statistics.failed;
		if (test.info.aborted) ++statistics.aborted;
	}
}}

namespace scutl {

	// The scutl entry point, which can be called by the user when ready to
	// run all the registered tests against the given reporter.
	void run(Reporter &reporter) {

		// Zero out statistics we will be collecting while running
		Test_Statistics statistics;
		statistics.count    = 0;
		statistics.started  = 0;
		statistics.complete = 0;
		statistics.passed   = 0;
		statistics.failed   = 0;
		statistics.aborted  = 0;

		// Grab a reference to our test list
		scutl::detail::Test::List &test_list = scutl::detail::Test::list();

		// Inform the reporter of how many tests we are going to run.
		statistics.count = test_list.size();
		reporter.report_test_count(statistics.count);

		// Run each test with the given reporter.
		for (
			scutl::detail::Test::List::iterator test_iterator = test_list.begin();
			test_iterator != test_list.end();
			++test_iterator
		) {
			scutl::detail::Test &test = **test_iterator;
			scutl::detail::run_test(test, reporter, statistics);
		}

		// Report the final summary
		reporter.report_test_summary(statistics);
	}

	Simple_Reporter::Simple_Reporter() {}

	Simple_Reporter::~Simple_Reporter() {}

	void Simple_Reporter::report_test_count(size_t count) {
		std::cout << "scutl: Running " << count << " tests ...\n";
	}

	void Simple_Reporter::report_test_started(const Test_Info &test_info) {
		std::cout
			<< "scutl: "
			<< test_info.file << ":"
			<< test_info.line << ":"
			<< test_info.name << ": starting\n";
		;
	}

	void Simple_Reporter::report_test_complete(const Test_Info &test_info) {
		std::cout
			<< "scutl: "
			<< test_info.file << ":"
			<< test_info.line << ":"
			<< test_info.name << ": completed\n"
		;
	}

	void Simple_Reporter::report_test_error(const Test_Info &test_info, const Error_Info &error_info) {
		std::ostringstream ss;
		ss
			<< "scutl: "
			<< error_info.file << ":"
			<< error_info.line << ":"
			<< test_info.name << ": ERROR: "
			<< error_info.expression << "\n"
		;
		std::cout << ss.str();
		std::cerr << ss.str();
	}

	void Simple_Reporter::report_test_summary(const Test_Statistics &statistics) {
		std::ostringstream ss;
		ss
			<< "scutl: SUMMARY: |"
			<< "count   " << "|"
			<< "started " << "|"
			<< "complete" << "|"
			<< "passed  " << "|"
			<< "failed  " << "|"
			<< "aborted " << "|"
			<< "\n"
			<< "scutl: SUMMARY: |"
			<< std::setw(8) << statistics.count    << "|"
			<< std::setw(8) << statistics.started  << "|"
			<< std::setw(8) << statistics.complete << "|"
			<< std::setw(8) << statistics.passed   << "|"
			<< std::setw(8) << statistics.failed   << "|"
			<< std::setw(8) << statistics.aborted  << "|"
			<< "\n"
		;
		std::cout << ss.str();
		std::cerr << ss.str();
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
