#pragma once

#include "FileSource.hpp"

#include <unordered_map>
#include <string>

namespace pulvis::fs
{
	class CFileSourceMemory : public IFileSource
	{
		public:

			void Store(const CFilePath& _path, CFileBuffer _buffer);

			EFileResult Read(const CFilePath& _path, CFileBuffer& _buffer) override;
			EFileResult Write(const CFilePath& _path, const CFileBuffer& _buffer) override;

			bool Exists(const CFilePath& _path) const override;
			bool IsReadOnly() const override { return false; }

			EFileResult GetFileInfo(const CFilePath& _path, SFileInfo& _info) const override;
			EFileResult ListDirectory(const CFilePath& _directory, std::vector<SFileInfo>& _files) const override;

			std::string_view Name() const override { return "Memory"; }

		private:

			std::unordered_map<CFilePath, CFileBuffer> m_Files;
	};
}