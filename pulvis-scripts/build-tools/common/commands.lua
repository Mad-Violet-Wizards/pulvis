function GenerateRttiCommand(script_path, target_path)
    if os.host() == "windows" then
        return "where py >nul 2>nul && py -3 \"" .. script_path .. "\" \"" .. target_path .. "\" --generate || python \"" .. script_path .. "\" \"" .. target_path .. "\" --generate"
    end

    return "python3 \"" .. script_path .. "\" \"" .. target_path .. "\" --generate"
end