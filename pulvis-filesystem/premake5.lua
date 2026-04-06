require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-filesystem"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-filesystem/src/**.hpp",
        "%{wks.location}/pulvis-filesystem/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-rtti", "pulvis-template-library" }
    dependson { "pulvis-rtti", "pulvis-template-library" }

project "pulvis-filesystem-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-filesystem/tests/**.hpp",
        "%{wks.location}/pulvis-filesystem/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-filesystem/tests",
        "%{wks.location}/pulvis-filesystem/tests/serialization",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath() .. "tests"),
    }

    links { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-filesystem" }
    dependson { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-filesystem" }