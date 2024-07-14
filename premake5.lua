workspace "Pelvis"
    configurations { "Debug", "Release" }
    platforms { "Windows", "MacOS-ARM", "MacOS-Intel" }

    filter { "system:Windows"}
        buildmessage("No supported yet.")

    filter("system:MacOSX", "architecture:ARM")
        defaultplatform "MacOS-ARM"
        include "./scripts/build/macos/arm/premake5_macos.lua"

    filter("system:MacOSX", "architecture:x86")
        buildmessage("No supported yet.")