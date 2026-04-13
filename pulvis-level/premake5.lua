project "pulvis-level"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-level/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-level/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-threads/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-rendering/src",
        PULVIS_ROOT .. "/pulvis-level/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows" 
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-core", "pulvis-filesystem", "pulvis-rendering", "pulvis-threads", "pulvis-rtti", "pulvis-events" }

    dependson { "pulvis-core", "pulvis-filesystem", "pulvis-rendering", "pulvis-threads", "pulvis-rtti", "pulvis-events" }
