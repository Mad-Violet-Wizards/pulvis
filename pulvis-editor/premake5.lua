require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-editor/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-editor/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-threads/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-rendering/src",
        PULVIS_ROOT .. "/pulvis-scriptable/src",
        PULVIS_ROOT .. "/pulvis-level/src",
        PULVIS_ROOT .. "/pulvis-ecs/src",
        PULVIS_ROOT .. "/pulvis-systems/src",
        PULVIS_ROOT .. "/pulvis-imgui/src",
        PULVIS_ROOT .. "/pulvis-game-engine/src",
        PULVIS_ROOT .. "/pulvis-editor/src",

        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }
        libdirs { PULVIS_ROOT .. "/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }

    filter {}
    links { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-events", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-scriptable", "pulvis-level", "pulvis-ecs", "pulvis-systems", "pulvis-imgui", "pulvis-game-engine" }
    dependson { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-events", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-scriptable", "pulvis-level", "pulvis-ecs", "pulvis-systems", "pulvis-imgui", "pulvis-game-engine" }