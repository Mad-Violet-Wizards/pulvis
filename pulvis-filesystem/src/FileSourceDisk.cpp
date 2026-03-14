#include "FileSourceDisk.hpp"

#include <fstream>

namespace pulvis::fs
{
	CFileSourceDisk::CFileSourceDisk(std::filesystem::path _root)
		: m_Root(std::filesystem::weakly_canonical(std::move(_root)))
	{

	}

	EFileResult CFileSourceDisk::Read(const CFilePath& _path, CFileBuffer& _buffer)
	{
		if (!Exists(_path))
		{
			return EFileResult::NotFound;
		}

		const std::filesystem::path absolute_path = ToAbsolute(_path);

		std::ifstream file(absolute_path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			return EFileResult::NotFound;

		const file_size_t file_size = static_cast<file_size_t>(file.tellg());
		file.seekg(0, std::ios::beg);

		_buffer.Resize(file_size);
		file.read(reinterpret_cast<char*>(_buffer.Data()), static_cast<std::streamsize>(file_size));

		return file.good() ? EFileResult::Success : EFileResult::IOError;
	}

	EFileResult CFileSourceDisk::Write(const CFilePath& _path, const CFileBuffer& _buffer)
	{
		if (!Exists(_path))
		{
			return EFileResult::NotFound;
		}

		const std::filesystem::path absolute_path = ToAbsolute(_path);

		std::ofstream file(absolute_path, std::ios::binary | std::ios::trunc);

		if (!file.is_open())
		{
			return EFileResult::AccessDenied;
		}

		file.write(reinterpret_cast<const char*>(_buffer.Data()), static_cast<std::streamsize>(_buffer.Size()));

		return file.good() ? EFileResult::Success : EFileResult::IOError;
	}

	bool CFileSourceDisk::Exists(const CFilePath& _path) const
	{
		return std::filesystem::exists(ToAbsolute(_path));
	}

	EFileResult CFileSourceDisk::GetFileInfo(const CFilePath& _path, SFileInfo& _info) const
	{
		if (!Exists(_path))
		{
			return EFileResult::NotFound;
		}

		const std::filesystem::path absolute_path = ToAbsolute(_path);

		_info.Path = _path;
		_info.IsDirectory = std::filesystem::is_directory(absolute_path);
		_info.Size = _info.IsDirectory ? 0 : static_cast<file_size_t>(std::filesystem::file_size(absolute_path));

		auto ftime = std::filesystem::last_write_time(absolute_path);
		_info.LastModified = static_cast<uint64_t>(ftime.time_since_epoch().count());

		return EFileResult::Success;

	}

	EFileResult CFileSourceDisk::ListDirectory(const CFilePath& _directory, std::vector<SFileInfo>& _files) const
	{
		if (!Exists(_directory))
		{
			return EFileResult::NotFound;
		}

		const std::filesystem::path absolute_path = ToAbsolute(_directory);

		for (const auto& entry : std::filesystem::directory_iterator(absolute_path))
		{
			SFileInfo info;
			info.Path = CFilePath(entry.path().lexically_relative(m_Root).string());
			info.IsDirectory = entry.is_directory();
			info.Size = info.IsDirectory ? 0 : static_cast<file_size_t>(entry.file_size());
			auto ftime = entry.last_write_time();
			info.LastModified = static_cast<uint64_t>(ftime.time_since_epoch().count());
			_files.push_back(std::move(info));
		}

		return _files.empty() ? EFileResult::NotFound : EFileResult::Success;
	}

	std::filesystem::path CFileSourceDisk::ToAbsolute(const CFilePath& _path) const
	{
		return m_Root / _path.Str();
	}
}
