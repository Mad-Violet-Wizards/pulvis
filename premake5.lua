local scriptPath = debug.getinfo(1, "S").source:match("@?(.*[/\\])") or "./"
package.path = package.path .. ";" .. scriptPath .. "?.lua"

require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.filepath"

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

    filter "system:macosx"
        defines { "MAC_OS" }
        architecture "ARM64"
        buildoptions { 
            "-Wunknown-attributes"   -- Disable warning: unknown attribute ignored
        }

    include "pulvis-template-library/premake5.lua"
    include "pulvis-rtti/premake5.lua"
    include "pulvis-core/premake5.lua"
    include "pulvis-events/premake5.lua"
    include "pulvis-threads/premake5.lua"
    include "pulvis-filesystem/premake5.lua"
    include "pulvis-rendering/premake5.lua"
    include "pulvis-level/premake5.lua"
    include "pulvis-game-engine/premake5.lua"
    include "pulvis-game/premake5.lua"
