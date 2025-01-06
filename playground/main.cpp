#if defined(WINDOWS_OS)
#include <vendor/common/include/catch2/catch2.hpp>

int main(int argc, const char* argv[])
{
	return Catch::Session().run(argc, argv);
}
#endif