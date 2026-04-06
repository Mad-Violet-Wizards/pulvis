#include "FileSourceMemory.hpp"

namespace pulvis::fs
{
	void CFileSourceMemory::Store(const CFilePath& _path, CFileBuffer _buffer)
	{
		m_Files[_path] = std::move(_buffer);
	}

	EFileResult CFileSourceMemory::Read(const CFilePath& _path, CFileBuffer& _buffer)
	{
		auto it = m_Files.find(_path);
		if (it == m_Files.end())
		{
			return EFileResult::NotFound;
		}

		_buffer = CFileBuffer(it->second.Data(), it->second.Size());
		return EFileResult::Success;
	}

	EFileResult CFileSourceMemory::Write(const CFilePath& _path, const CFileBuffer& _buffer)
	{
		auto it = m_Files.find(_path);
		if (it == m_Files.end())
		{
			return EFileResult::NotFound;
		}

		it->second = _buffer;
		return EFileResult::Success;
	}

	bool CFileSourceMemory::Exists(const CFilePath& _path) const
	{
		return m_Files.find(_path) != m_Files.end();
	}

	EFileResult CFileSourceMemory::GetFileInfo(const CFilePath& _path, SFileInfo& _info) const
	{
		auto it = m_Files.find(_path);
		if (it == m_Files.end())
		{
			return EFileResult::NotFound;
		}

		_info.Path = _path;
		_info.Size = it->second.Size();
		_info.LastModified = 0;
		_info.IsDirectory = false;
		return EFileResult::Success;
	}

	EFileResult CFileSourceMemory::ListDirectory(const CFilePath& _directory, std::vector<SFileInfo>& _files) const
	{
		std::string prefix = _directory.Str();
		if (!prefix.empty() && prefix.back() != '/')
		{
			prefix += '/';
		}

		for (const auto& [path, buffer] : m_Files)
		{
			if (!path.StartsWith(prefix))
			{
				continue;
			}

			std::string_view remainder(path.Str());
			remainder.remove_prefix(prefix.size());

			if (remainder.find('/') != std::string_view::npos)
				continue;

			SFileInfo info;
			info.Path = path;
			info.Size = buffer.Size();
			info.IsDirectory = false;
			info.LastModified = 0;
			_files.push_back(std::move(info));
		}

		return _files.empty() ? EFileResult::NotFound : EFileResult::Success;
	}
}