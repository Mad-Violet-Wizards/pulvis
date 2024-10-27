#include <iostream>

#include <vendor/common/include/catch2/catch2.hpp>

int main(int argc, char* argv[])
{
	std::cout << "Press Enter to Continue...";
	std::cin.ignore();
	return Catch::Session().run(argc, argv);
}