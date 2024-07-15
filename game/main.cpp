#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/core/Assert.hpp"
#include "engine/filesystem/Utils.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] Zebys kurwo miala szczesliwe zycie, tego Ci zycze z calego serduszka.\n");
    
    Singleton<engine::core::AssertManager>::Init();

#ifdef _WIN32
    engine::fs::InitializeAppdataPath();
#endif

    return 0;
}
