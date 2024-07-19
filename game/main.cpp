#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/core/Assert.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/rendering/Window.hpp"
#include "engine/rendering/Context.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] Zebys kurwo miala szczesliwe zycie, tego Ci zycze z calego serduszka.\n");
    
    Singleton<engine::core::AssertManager>::Init();

#ifdef _WIN32
    engine::fs::InitializeAppdataPath();
#endif

    engine::rendering::Context vk_context;
    std::vector<std::string> vk_instance_layers;
    vk_context.FillInstanceLayers(vk_instance_layers, true);

		engine::rendering::Window window(800, 600, "Pulvis Engine Alpha (1.0.0)");
     
    while (!window.ShouldClose())
    {
      glfwPollEvents();
    }

    return 0;
}
