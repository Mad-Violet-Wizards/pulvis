require "pulvis-scripts.build.common.cache"
require "pulvis-scripts.build.common.commands"
require "pulvis-scripts.build.common.filepath"

project "pulvis-filesystem"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions {
        "/utf-8"
    }

    files {
        "%{wks.location}/pulvis-filesystem/src/**.hpp",
        "%{wks.location}/pulvis-filesystem/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-rtti", "pulvis-template-library" }
    dependson { "pulvis-rtti", "pulvis-template-library" }

project "pulvis-filesystem-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions {
        "/utf-8"
    }

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
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath() .. "tests"),
    }

    links { "pulvis-filesystem", "pulvis-rtti", "pulvis-template-library" }
    dependson { "pulvis-filesystem", "pulvis-rtti", "pulvis-template-library" }