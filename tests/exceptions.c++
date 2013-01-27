#include <scutl.h++>
#include <stdexcept>

TEST(no_exception) {
	EXPECT(true);
}

TEST(runtime_error) {
	throw std::runtime_error("runtime error occured");
	EXPECT(true);
}

TEST(string_error) {
	throw std::string("string error occured");
	EXPECT(true);
}

TEST(c_string_error) {
	throw "C string error occured";
	EXPECT(true);
}

TEST(unknown_exception) {
	throw 0;
	EXPECT(true);
}
