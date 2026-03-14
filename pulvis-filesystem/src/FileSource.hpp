#pragma once

#include "FileTypes.hpp"
#include "FileBuffer.hpp"
#include "FilePath.hpp"

#include <vector>
#include <string_view>

namespace pulvis::fs
{
	struct SFileInfo
	{
		CFilePath Path;
		file_size_t Size = 0;
		uint64_t LastModified = 0;
		bool IsDirectory = false;
	};

	class IFileSource
	{
		public:

			virtual ~IFileSource() = default;

			virtual EFileResult Read(const CFilePath& _path, CFileBuffer& _buffer) = 0;
			virtual EFileResult Write(const CFilePath& _path, const CFileBuffer& _buffer) = 0;

			virtual bool Exists(const CFilePath& _path) const = 0;
			virtual bool IsReadOnly() const = 0;

			virtual EFileResult GetFileInfo(const CFilePath& _path, SFileInfo& _info) const = 0;
			virtual EFileResult ListDirectory(const CFilePath& _directory, std::vector<SFileInfo>& _files) const = 0;

			virtual std::string_view Name() const = 0;
	};
};