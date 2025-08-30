#include <vendor/common/include/catch2/catch2.hpp>

#include "tests/tests_rtti_autogen.hpp"

int main(int argc, const char* argv[])
{
	RegisterRTTI_tests();
	return Catch::Session().run(argc, argv);
}
