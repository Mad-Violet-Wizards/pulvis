project "pulvis-level"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-level/src/**.hpp",
        "%{wks.location}/pulvis-level/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-events/src",
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-level/src",
        "%{wks.location}/pulvis-vendor/common/include/"
    }

    filter "system:windows" 
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-core", "pulvis-filesystem", "pulvis-rendering", "pulvis-threads", "pulvis-rtti", "pulvis-events" }

    dependson { "pulvis-core", "pulvis-filesystem", "pulvis-rendering", "pulvis-threads", "pulvis-rtti", "pulvis-events" }
