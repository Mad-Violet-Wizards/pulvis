require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-events"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-events/src/**.hpp",
        "%{wks.location}/pulvis-events/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-events/src"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core" }

project "pulvis-events-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-events/tests/**.hpp",
        "%{wks.location}/pulvis-events/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-events/src",
        "%{wks.location}/pulvis-events/tests"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}

    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-events" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-events" }