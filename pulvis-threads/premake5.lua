project "pulvis-threads"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-threads/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-threads/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-threads/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library", "pulvis-core" }
    dependson { "pulvis-template-library", "pulvis-core" }

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
    links { "pulvis-template-library", "pulvis-core", "pulvis-threads" }
    dependson { "pulvis-template-library", "pulvis-core", "pulvis-threads" }