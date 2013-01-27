#include <scutl.h++>
#include <stdexcept>

TEST(no_exception) {
	EXPECT(true);
}

TEST(runtime_error) {
	std::cerr << "Expect an exception with \"runtime error occured\"\n";
	throw std::runtime_error("runtime error occured");
	EXPECT(true);
}

TEST(unknown_exception) {
	std::cerr << "Expect an unknown exception\n";
	throw 0;
	EXPECT(true);
}
