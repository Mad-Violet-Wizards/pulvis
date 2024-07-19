workspace "Pulvis"
    configurations { "Debug", "Release" }
    platforms { "Windows", "MacOS-ARM", "MacOS-Intel" }

    filter { "system:Windows"}
        defaultplatform "Windows"
        include "./scripts/build/windows/premake5_windows.lua"

    filter("system:MacOSX", "architecture:ARM")
        defaultplatform "MacOS-ARM"
        include "./scripts/build/macos/arm/premake5_macos.lua"

    filter("system:MacOSX", "architecture:x86")
        buildmessage("No supported yet.")