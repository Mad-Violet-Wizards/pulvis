project "pulvis-game-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-game-engine/src/**.hpp",
        "%{wks.location}/pulvis-game-engine/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-level/src",
        "%{wks.location}/pulvis-game-engine/src",

        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }
        libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }

    filter {}
    links { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level" }

    dependson { "pulvis-template-library", "pulvis-core", "pulvis-rtti", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level" }
