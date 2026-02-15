project "pulvis-template-library"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }
    
    files {
        "%{wks.location}/pulvis-template-library/src/**.hpp",
        "%{wks.location}/pulvis-template-library/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

project "pulvis-template-library-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }

    files {
        "%{wks.location}/pulvis-template-library/tests/**.hpp",
        "%{wks.location}/pulvis-template-library/tests/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/tests",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }