workspace "Pulvis"
    configurations { "Debug", "Release" }

    local premakeFile = debug.getinfo(1, "S").source:sub(2)
    local fullPath = path.getabsolute(premakeFile)
    local workspace_root = path.getabsolute(path.join(fullPath, "../"))  -- => pulvis/

    _G.WORKSPACE_ROOT = workspace_root

    if os.target() == "windows" then
        local engine_rtti_cmd = string.format('py "%s/scripts/rtti/main.py" "%s/engine" --generate', workspace_root, workspace_root)
        local game_rtti_cmd = string.format('py "%s/scripts/rtti/main.py" "%s/game" --generate', workspace_root, workspace_root)
        local playground_rtti_cmd = string.format('py "%s/scripts/rtti/main.py" "%s/playground" --generate', workspace_root, workspace_root)
        local tests_rtti_cmd = string.format('py "%s/scripts/rtti/main.py" "%s/tests" --generate', workspace_root, workspace_root)
        os.execute(engine_rtti_cmd)
        os.execute(game_rtti_cmd)
        os.execute(playground_rtti_cmd)
        os.execute(tests_rtti_cmd)
        include "./scripts/build/windows/premake5_windows.lua"
        buildmessage("Including windows build.")
    elseif os.target() == "macosx" then
        include "./scripts/build/macos/arm/premake5_macos.lua"
        print("Including macos-arm build.")
    else
        buildmessage("Unsupported system.")
    end
