---------------------------------------------------------------
-- RTTI 
---------------------------------------------------------------
require "pulvis-scripts.build.common.cache"
require "pulvis-scripts.build.common.filepath"

PROJECT_ABSOLUTE_PATH = GetScriptPath()
RTTI_GENERATION_SCRIPT_ABSOLUTE_PATH = PROJECT_ABSOLUTE_PATH .. "pulvis-scripts/rtti/main.py"

---------------------------------------------------------------
-- PREMAKE
---------------------------------------------------------------
workspace "pulvis"
    configurations { "Debug", "Release"}

    filter "configurations:Debug"
        defines { "DEBUG", "FMT_HEADER_ONLY" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

    filter "system:windows"
        defines { "WINDOWS_OS" }
        buildoptions { 
            "/wd5030"   -- Disable warning C5030: attribute not recognized
        }

    include "pulvis-template-library/premake5.lua"
    include "pulvis-rtti/premake5.lua"
    include "pulvis-core/premake5.lua"
    include "pulvis-rendering/premake5.lua"
    include "pulvis-level/premake5.lua"
    include "pulvis-game/premake5.lua"
