#include <iostream>
#include "engine/core/Assert.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] Hello world.\n");

    core::AssertUnix unix_assert("this", 1);
    core::AssertWindows win_assert("this", 1);

    unix_assert.PrintCallstack();
    win_assert.PrintCallstack();

    std::cout << "T" << "\n";
    return 0;
}
