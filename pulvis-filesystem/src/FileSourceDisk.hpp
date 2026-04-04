#pragma once
#pragma once

#include "FileSource.hpp"

#include <Filesystem>

/* CFileSourceDisk - File source backed by a physical directory on disk.
	 Constructor accepts physical root (either relative or absolute)
	 Relative is resolved against the current working directory of the process. 
	 
	 Example:
		* CFileSourceDisk disk_source("game/assets"); -> <current_working_directory>/game/assets
	  * CFileSourceDisk disk_source("C:/Program Files/Windows/"); -> C:/Program Files/Windows/
*/

namespace pulvis::fs
{
	class CFileSourceDisk : public IFileSource
	{
		public:

			explicit CFileSourceDisk(std::filesystem::path _root, bool read_only);

			EFileResult Read(const CFilePath& _path, CFileBuffer& _buffer) override;
			EFileResult Write(const CFilePath& _path, const CFileBuffer& _buffer) override;

			bool Exists(const CFilePath& _path) const override;
			bool IsReadOnly() const override { return m_ReadOnly; }

			EFileResult GetFileInfo(const CFilePath& _path, SFileInfo& _info) const override;
			EFileResult ListDirectory(const CFilePath& _directory, std::vector<SFileInfo>& _files) const override;

			std::string_view Name() const override { return "Disk"; }
			const std::filesystem::path& Root() const { return m_Root; }

		private:

			std::filesystem::path ToAbsolute(const CFilePath& _path) const;


		private:

			std::filesystem::path m_Root;
			bool m_ReadOnly;
	};
}