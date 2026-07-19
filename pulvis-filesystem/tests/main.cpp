#include <catch2/catch2.hpp>

#include "PulvisFilesystemTests_rtti_autogen.hpp"

int main(int argc, const char* argv[])
{
		RegisterRTTI_PulvisFilesystemTests();
		return Catch::Session().run(argc, argv);
}