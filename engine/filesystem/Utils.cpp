#include "engine/engine_pch.hpp"
#include "Utils.hpp"

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