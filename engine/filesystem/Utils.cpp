#include "engine/engine_pch.hpp"
#include "Utils.hpp"

#if defined(_WIN32) | defined(_WIN64)
std::string engine::fs::GetEnginePath()
{
	char* appdata_buffer = nullptr;
	size_t required_buffer_size = 0;

	errno_t error = _dupenv_s(&appdata_buffer, &required_buffer_size, "APPDATA");
	
	if (error != 0)
	{
		std::cerr << "Failed to get APPDATA environment variable.\n";
		return "";
	}
	
	std::string res(appdata_buffer);
	free(appdata_buffer);
	return res;
}
#else
#include <pwd.h>
#include <unistd.h>

std::string engine::fs::GetEnginePath()
{
	const char* home_dir;

	if ((home_dir = getenv("HOME")) == NULL)
	{
		home_dir = getpwuid(getuid())->pw_dir;
	}

	std::string res(home_dir);
	res += "/Library/Preferences";

	return res;
}
#endif