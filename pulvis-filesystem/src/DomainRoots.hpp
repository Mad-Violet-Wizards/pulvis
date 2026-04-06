#pragma once

#include "FileTypes.hpp"
#include <array>
#include <string>
#include <filesystem>

namespace pulvis::fs
{
	/* 
		In debug builds path are resolved relative to project workspace.
		In release builds path are resolved relative to the executable.
	*/

	class CDomainRoots
	{
		public:

			CDomainRoots(const std::string& _app_name);
			CDomainRoots(const CDomainRoots&) = delete;
			CDomainRoots& operator=(const CDomainRoots&) = delete;

			const std::filesystem::path& GetRoot(EDomain _domain) const;
			bool IsValid(EDomain _domain) const;

			void OverrideRoot(EDomain _domain, std::filesystem::path _new_root);

		private:

			static std::filesystem::path ResolveUserDataPath(const std::string& _app_name);

		private:

			std::array<std::filesystem::path, DOMAIN_COUNT> m_Roots;
	};
}