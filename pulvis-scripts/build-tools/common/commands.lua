function GenerateRttiCommand(script_path, target_path)
    return "python3 " .. script_path .. " " .. target_path .. " --generate"
end
