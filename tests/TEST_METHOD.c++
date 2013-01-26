#include <scutl.h++>

struct Empty {};

TEST_METHOD(Empty, TEST_METHOD_works) {}

TEST_METHOD(Empty, TEST_METHOD_duplicate_test_names_are_ok) {}
TEST_METHOD(Empty, TEST_METHOD_duplicate_test_names_are_ok) {}
TEST_METHOD(Empty, TEST_METHOD_duplicate_test_names_are_ok) {}

namespace xyz {
	TEST_METHOD(Empty, TEST_METHOD_works_in_an_arbitrary_namespace) {}
}

namespace Unmodified {
	struct Fixture {
		Fixture() : x(50) {}
		size_t x;
	};

	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 1;}
	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 8;}
	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 3;}
	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 0;}
	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 4;}
	TEST_METHOD(Fixture, fixture_is_unmodified) { REQUIRE(x == 50); x = 2;}
}

namespace Increment_Decrement {
	struct Fixture {
		Fixture() : x(10) {}
		size_t x;
	};

	TEST_METHOD(Fixture, increment) {
		REQUIRE(x == 10);
		REQUIRE(++x == 11);
		REQUIRE(x == 11);
		REQUIRE(x++ == 11);
		REQUIRE(x == 12);
	}

	TEST_METHOD(Fixture, decrement) {
		ASSERT(x == 10);
		ASSERT(--x == 9);
		ASSERT(x == 9);
		ASSERT(x-- == 9);
		ASSERT(x == 8);
	}
}
