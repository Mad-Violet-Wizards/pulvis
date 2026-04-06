#include "Filesystem.hpp"

#include <cstdlib>

#include "Assert.hpp"

std::filesystem::path pulvis::core::GetAssetsPath()
{
#if defined(DEBUG)
	std::filesystem::path current_path = std::filesystem::current_path() / ".." / "pulvis-assets";
	return current_path.lexically_normal();
#endif
	return {};
}

std::filesystem::path pulvis::core::GetWorkspacePath()
{
#if defined(WINDOWS_OS)
	char* appdata_buffer = nullptr;
	size_t required_buffer_size = 0;

	const errno_t error = _dupenv_s(&appdata_buffer, &required_buffer_size, "APPDATA");

	if (error != 0 || appdata_buffer == nullptr || required_buffer_size == 0)
	{
		if (appdata_buffer)
		{
			free(appdata_buffer);
		}

		ASSERT(false, "Failed to get APPDATA enviroment variable.");
		return {};
	}

	std::filesystem::path p(appdata_buffer);
	free(appdata_buffer);
	return p;
#endif
	return {};
}

std::filesystem::path pulvis::core::GetWorkspacePath(const std::string& path_append)
{
	return GetWorkspacePath() / path_append;
}

std::filesystem::path pulvis::core::GetAssetsPath(const std::string& path_append)
{
	return GetAssetsPath() / path_append;
}

