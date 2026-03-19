project "pulvis-game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }

    files {
        "%{wks.location}/pulvis-game/src/**.hpp",
        "%{wks.location}/pulvis-game/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-game/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-level/src",

        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    defines { "GLFW_DLL" }

    libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }

    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level", "glfw3dll" }

    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level" }

    filter "system:windows"
        postbuildcommands {
            "{COPY} %{wks.location}/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }
