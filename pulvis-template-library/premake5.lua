project "pulvis-template-library"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files {
        PULVIS_ROOT .. "/pulvis-template-library/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-template-library/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

filter {}

project "pulvis-template-library-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-template-library/tests/**.hpp",
        PULVIS_ROOT .. "/pulvis-template-library/tests/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/tests",
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library" }
    dependson { "pulvis-template-library" }