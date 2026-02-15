project "pulvis-level"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }

    files {
        "%{wks.location}/pulvis-level/src/**.hpp",
        "%{wks.location}/pulvis-level/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-level/src",

        "${wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }

    links { "pulvis-core" }

    dependson { "pulvis-core"}
