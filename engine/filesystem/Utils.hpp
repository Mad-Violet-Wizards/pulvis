#pragma once

namespace engine
{
	namespace fs
	{
#ifdef _WIN32
		static char* s_AppdataPath = nullptr;
		static void InitializeAppdataPath()
		{
			char* appdata_buffer = nullptr;
			size_t required_buffer_size = 0;

			errno_t error = _dupenv_s(&appdata_buffer, &required_buffer_size, "APPDATA");

			if (error != 0)
			{
				std::cerr << "Failed to get APPDATA environment variable.\n";
				return;
			}

			s_AppdataPath = appdata_buffer;
			std::cout << "DEBUG: APPDATA path: " << s_AppdataPath << "\n";

			free(appdata_buffer);
		}
#endif
	}
}