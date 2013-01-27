#include <scutl.h++>

TEST(EXPECT_true_then_false) {
	EXPECT(true);
	std::cerr << "Expect an EXPECT(false) error\n";
	EXPECT(false);
}

TEST(EXPECT_false_then_true) {
	std::cerr << "Expect an EXPECT(false) error\n";
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
	std::cerr << "Expect an EXPECT(x+x == 3) error\n";
	EXPECT(x+x == 3);
	EXPECT(1+1 == 2);
}

void function_calling_expect(int x) {
	EXPECT( x  == 1);
}

TEST(call_expect_from_a_function) {
	int x = 1;
	function_calling_expect(x);
}
