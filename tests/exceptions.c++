#include <scutl.h++>
#include <stdexcept>

TEST(no_exception) {
	ASSERT(true);
}

TEST(runtime_error) {
	throw std::runtime_error("runtime error occured");
	ASSERT(true);
}

TEST(string_error) {
	throw std::string("string error occured");
	ASSERT(true);
}

TEST(c_string_error) {
	throw "C string error occured";
	ASSERT(true);
}

TEST(unknown_exception) {
	throw 0;
	ASSERT(true);
}
