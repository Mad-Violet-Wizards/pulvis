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
			~Filesystem();

			void Mount();
			void Unmount();
			bool IsMounted() const;

			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, std::shared_ptr<IFileDataModel>* _file_data_model, EFileMode _open_mode);
			std::optional<CFileHandle> OpenFile(const std::string& _relative_path, EFileMode _open_mode);

			void GetFilenamesInDirectory(const std::string& _relative_path, std::vector<std::string>& _out_filenames_list) const;

			bool GetFileExists(const std::string& _relative_path) const;

			std::vector<std::string>::const_iterator FileListBegin() const;
			std::vector<std::string>::const_iterator FileListEnd() const;

			static std::string GetFilename(const std::string& _filepath);
			static std::string GetFileExtension(const std::string& _filename);

		private:

			bool GetCreateFileIfNotExists(EFileMode _open_mode) const;

		private:

			class Impl;
			Impl* m_Impl = nullptr;
	};
}
}
