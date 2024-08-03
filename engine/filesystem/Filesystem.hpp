#pragma once

#include <filesystem>
#include <optional>

#include "FileHandle.hpp"

namespace engine
{
namespace fs
{
	class PULVIS_API Filesystem
	{
		public:

			Filesystem(const std::string& _name, const std::string& _absolute_path);
			~Filesystem() = default;

			void Mount();
			void Unmount();

			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model, EFileMode _open_mode);
			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, EFileMode _open_mode);

			bool GetIsMounted() const { return m_Mounted; }

		private:

			bool GetCreateFileIfNotExists(EFileMode _open_mode) const;

		private:

			bool m_Mounted;
			std::string m_Name;
			std::filesystem::path m_AbsolutePath;

			std::vector<std::string> m_MountedFilelist;
	};
}
}
