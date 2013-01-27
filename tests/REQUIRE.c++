#include <scutl.h++>

TEST(REQUIRE_true_then_false) {
	REQUIRE(true);
	std::cerr << "Expect a REQUIRE(false) error\n";
	REQUIRE(false);
}

TEST(REQUIRE_false_then_true) {
	std::cerr << "Expect a REQUIRE(false) error\n";
	REQUIRE(false);
	REQUIRE(true);
}

TEST(one_plus_one_passing) {
	int x = 1;
	REQUIRE( x  == 1);
	REQUIRE(x+x == 2);
	REQUIRE(1+1 == 2);
}

TEST(one_plus_one_failing) {
	int x = 1;
	REQUIRE( x  == 1);
	std::cerr << "Expect a REQUIRE(x+x == 3) error\n";
	REQUIRE(x+x == 3);
	REQUIRE(1+1 == 2);
}

void function_calling_require(int x) {
	REQUIRE(x == 1);
}

TEST(call_require_from_a_function) {
	int x = 1;
	function_calling_require(x);
}
