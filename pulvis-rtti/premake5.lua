require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-rtti"
    kind "SharedLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    -- Built as a DLL so the global RTTI registries are a single shared
    -- instance across the engine and any future editor/game/plugin DLLs.
    defines { "PULVIS_RTTI_EXPORTS" }

    files {
        PULVIS_ROOT .. "/pulvis-rtti/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-rtti/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }

project "pulvis-rtti-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-rtti/tests/**.hpp",
        PULVIS_ROOT .. "/pulvis-rtti/tests/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/tests",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-scriptable/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath() .. "tests")
    }
    links { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }