require "pulvis-scripts.build.common.cache"
require "pulvis-scripts.build.common.commands"
require "pulvis-scripts.build.common.filepath"

project "pulvis-rtti"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }
    
    files {
        "%{wks.location}/pulvis-rtti/src/**.hpp",
        "%{wks.location}/pulvis-rtti/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }

project "pulvis-rtti-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }

    files {
        "%{wks.location}/pulvis-rtti/tests/**.hpp",
        "%{wks.location}/pulvis-rtti/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/tests",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath())
    }
    links { "pulvis-template-library", "pulvis-rtti" }
    dependson { "pulvis-template-library", "pulvis-rtti" }