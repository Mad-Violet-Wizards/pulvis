require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

-- The pulvis-filesystem library has been merged into the consolidated
-- pulvis-engine target. Only the standalone test app remains here.
project "pulvis-filesystem-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-filesystem/tests/**.hpp",
        PULVIS_ROOT .. "/pulvis-filesystem/tests/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-filesystem/tests",
        PULVIS_ROOT .. "/pulvis-filesystem/tests/serialization",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    prebuildcommands {
        GenerateRttiCommand(RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH, GetScriptPath() .. "tests"),
    }

    links { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
