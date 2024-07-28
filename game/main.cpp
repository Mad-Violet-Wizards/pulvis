#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/rendering/Window.hpp"
#include "engine/rendering/Context.hpp"
#include "engine/rendering/Utility.hpp"

#include <vulkan/vulkan.h>

int main(int argc, const char* argv[])
{
    printf("[Main] Zebys kurwo miala szczesliwe zycie, tego Ci zycze z calego serduszka.\n");
    
    Singleton<engine::core::CAssertManager>::Init();

#ifdef _WIN32
    engine::fs::InitializeAppdataPath();
#endif


    std::vector<std::string> vk_requested_instance_layers
    {
#ifdef DEBUG
      "VK_LAYER_KHRONOS_validation"
#endif
    };
    engine::rendering::CContext vk_context(vk_requested_instance_layers);

		engine::rendering::CWindow window(800, 600, "Pulvis Engine Alpha (1.0.0)");
     
    while (!window.ShouldClose())
    {
      glfwPollEvents();
    }

    return 0;
}
