#pragma once

#include <string>
#include <cstdint>

#include "RendererType.hpp"


namespace pulvis::game_engine
{
    struct SEngineConfig
    {
        pulvis::rendering::ERendererType RendererType = pulvis::rendering::ERendererType::OpenGL;
        std::string AppName = "pulvis";
        std::string GameAssetsPath = "";  // Relative to workspace root (e.g. "stardust/stardust-assets"). If empty, defaults to "{AppName}-assets".
        uint32_t WindowWidth = 800;
        uint32_t WindowHeight = 600;
        std::string WindowTitle = "Pulvis";
    };
}
