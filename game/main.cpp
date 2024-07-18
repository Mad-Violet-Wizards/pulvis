#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/core/Assert.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/rendering/Window.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] Zebys kurwo miala szczesliwe zycie, tego Ci zycze z calego serduszka.\n");
    
    Singleton<engine::core::AssertManager>::Init();

#ifdef _WIN32
    engine::fs::InitializeAppdataPath();
#endif

		engine::render::Window window(800, 600, "Pulvis Engine Alpha (1.0.0)");
     
    while (!window.ShouldClose())
    {
      glfwPollEvents();
    }

    return 0;
}
