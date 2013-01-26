#include <scutl.h++>

TEST(REQUIRE_true_then_false) {
	REQUIRE(true);
	REQUIRE(false);
}

TEST(REQUIRE_false_then_true) {
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
	REQUIRE(x+x == 3);
	REQUIRE(1+1 == 2);
}
