workspace "Pulvis"
    configurations { "Debug", "Release" }

    if os.target() == "windows" then
        include "./scripts/build/windows/premake5_windows.lua"
        buildmessage("Including windows build.")
    elseif os.target() == "macosx" then
        include "./scripts/build/macos/arm/premake5_macos.lua"
        print("Including macos-arm build.")
    else
        buildmessage("Unsupported system.")
    end