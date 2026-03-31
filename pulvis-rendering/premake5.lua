project "pulvis-rendering"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }
    
    files { 
        "%{wks.location}/pulvis-rendering/src/**.hpp", 
        "%{wks.location}/pulvis-rendering/src/**.cpp", 
        "%{wks.location}/pulvis-rendering/src/**.c"
    }

    includedirs { "%{wks.location}/pulvis-rendering/src",
                  "%{wks.location}/pulvis-core/src",
                  "%{wks.location}/pulvis-filesystem/src",
                  "%{wks.location}/pulvis-vendor/common/include/",
                  "%{wks.location}/pulvis-vendor/windows/include/" }

    libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }

    defines { "GLFW_DLL" }

    links { "pulvis-core","pulvis-filesystem","glfw3dll"}
    dependson { "pulvis-core", "pulvis-filesystem", "pulvis-rendering", "pulvis-level" }

    postbuildcommands {
        "{COPY} %{wks.location}/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
    }

project "pulvis-rendering-playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    files { 
        "%{wks.location}/pulvis-rendering/playground/include/**.hpp", 
        "%{wks.location}/pulvis-rendering/playground/src/**.cpp" 
    }

    includedirs { 
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/",
    }

    libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }

    links { "glfw3dll", "pulvis-rendering", "pulvis-filesystem", "pulvis-core" }
    dependson { "glfw3dll", "pulvis-rendering", "pulvis-filesystem", "pulvis-core" }
