project "pulvis-core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    buildoptions { 
        "/utf-8" -- FMT library requires UTF-8 encoding
    }
    
    files {
        "%{wks.location}/pulvis-core/src/**.hpp",
        "%{wks.location}/pulvis-core/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-vendor/common/include/",
        "%{wks.location}/pulvis-vendor/windows/include/"
    }