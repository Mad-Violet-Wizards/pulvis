require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

---------------------------------------------------------------
-- pulvis-engine
--
-- Single consolidated engine target. It merges the sources of
-- what used to be the separate static libraries:
--   core, events, threads, scriptable, filesystem, systems,
--   ecs, rendering, imgui, level, game-engine
-- into one StaticLib. pulvis-template-library and pulvis-rtti
-- remain as separate targets (rtti is a shared library / DLL).
---------------------------------------------------------------
local ENGINE_MODULES = {
    "pulvis-core",
    "pulvis-events",
    "pulvis-threads",
    "pulvis-scriptable",
    "pulvis-filesystem",
    "pulvis-systems",
    "pulvis-ecs",
    "pulvis-rendering",
    "pulvis-imgui",
    "pulvis-level",
    "pulvis-game-engine",
}

project "pulvis-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    -- Glob the sources of every merged module.
    for _, module in ipairs(ENGINE_MODULES) do
        files {
            PULVIS_ROOT .. "/" .. module .. "/src/**.hpp",
            PULVIS_ROOT .. "/" .. module .. "/src/**.cpp",
            PULVIS_ROOT .. "/" .. module .. "/src/**.c",
        }
    end

    -- Union of the include roots of every merged module plus the
    -- two standalone libraries (template-library, rtti) and vendor.
    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-threads/src",
        PULVIS_ROOT .. "/pulvis-scriptable/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-systems/src",
        PULVIS_ROOT .. "/pulvis-ecs/src",
        PULVIS_ROOT .. "/pulvis-rendering/src",
        PULVIS_ROOT .. "/pulvis-imgui/src",
        PULVIS_ROOT .. "/pulvis-level/src",
        PULVIS_ROOT .. "/pulvis-game-engine/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/",
    }

    -- Generate RTTI code for the merged sources that use reflection
    -- attributes (currently only pulvis-systems carries markers).
    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, PULVIS_ROOT .. "/pulvis-systems/src"),
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }
        libdirs { PULVIS_ROOT .. "/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }
        links { "glfw3dll" }
        postbuildcommands {
            "{COPY} " .. PULVIS_ROOT .. "/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "glfw", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    links { "pulvis-template-library", "pulvis-rtti" }
    dependson { "pulvis-template-library", "pulvis-rtti" }
