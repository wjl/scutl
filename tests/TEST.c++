#include <scutl.h++>

TEST(TEST_works) {}

TEST(duplicate_test_names_are_ok) {}
TEST(duplicate_test_names_are_ok) {}
TEST(duplicate_test_names_are_ok) {}

namespace xyz {
	TEST(TEST_works_in_an_arbitrary_namespace) {}
}
