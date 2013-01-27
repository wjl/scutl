#include <scutl.h++>

TEST(EXPECT_true_then_false) {
	EXPECT(true);
	EXPECT(false);
}

TEST(EXPECT_false_then_true) {
	EXPECT(false);
	EXPECT(true);
}

TEST(one_plus_one_passing) {
	int x = 1;
	EXPECT( x  == 1);
	EXPECT(x+x == 2);
	EXPECT(1+1 == 2);
}

TEST(one_plus_one_failing) {
	int x = 1;
	EXPECT( x  == 1);
	EXPECT(x+x == 3);
	EXPECT(1+1 == 2);
}
