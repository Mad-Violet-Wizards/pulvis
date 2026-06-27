-- The pulvis-threads library has been merged into the consolidated
-- pulvis-engine target. Only the standalone test app remains here.
project "pulvis-threads-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-threads/tests/**.hpp",
        PULVIS_ROOT .. "/pulvis-threads/tests/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-threads/tests",
        PULVIS_ROOT .. "/pulvis-threads/src",
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
