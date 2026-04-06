project "pulvis-template-library"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files {
        "%{wks.location}/pulvis-template-library/src/**.hpp",
        "%{wks.location}/pulvis-template-library/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

filter {}

project "pulvis-template-library-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-template-library/tests/**.hpp",
        "%{wks.location}/pulvis-template-library/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/tests",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }