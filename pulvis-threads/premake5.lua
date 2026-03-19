project "pulvis-threads"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8"
    }

    files {
        "%{wks.location}/pulvis-threads/src/**.hpp",
        "%{wks.location}/pulvis-threads/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-template-library", "pulvis-core" }
    dependson { "pulvis-template-library", "pulvis-core" }

project "pulvis-threads-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8"
    }

    files {
        "%{wks.location}/pulvis-threads/tests/**.hpp",
        "%{wks.location}/pulvis-threads/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-threads/tests",
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-template-library", "pulvis-core", "pulvis-threads" }
    dependson { "pulvis-template-library", "pulvis-core", "pulvis-threads" }