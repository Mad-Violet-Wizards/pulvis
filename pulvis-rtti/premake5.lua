require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-rtti"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files {
        "%{wks.location}/pulvis-rtti/src/**.hpp",
        "%{wks.location}/pulvis-rtti/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }

project "pulvis-rtti-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-rtti/tests/**.hpp",
        "%{wks.location}/pulvis-rtti/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/tests",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath() .. "tests")
    }
    links { "pulvis-template-library", "pulvis-rtti" }
    dependson { "pulvis-template-library", "pulvis-rtti" }