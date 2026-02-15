#pragma once

#include <filesystem>

namespace pulvis::core
{
	std::filesystem::path GetAssetsPath();
	std::filesystem::path GetWorkspacePath();

	std::filesystem::path GetAssetsPath(const std::string& path_append);
	std::filesystem::path GetWorkspacePath(const std::string& path_append);
}