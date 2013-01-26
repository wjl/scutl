#include <scutl.h++>

TEST(ASSERT_true_then_false) {
	ASSERT(true);
	ASSERT(false);
}

TEST(ASSERT_false_then_true) {
	ASSERT(false);
	ASSERT(true);
}

TEST(one_plus_one_passing) {
	int x = 1;
	ASSERT( x  == 1);
	ASSERT(x+x == 2);
	ASSERT(1+1 == 2);
}

TEST(one_plus_one_failing) {
	int x = 1;
	ASSERT( x  == 1);
	ASSERT(x+x == 3);
	ASSERT(1+1 == 2);
}
