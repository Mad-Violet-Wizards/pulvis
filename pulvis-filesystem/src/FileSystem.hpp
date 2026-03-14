#pragma once

#include "MountSystem.hpp"
#include "FileBuffer.hpp"
#include "FilePath.hpp"
#include "FileTypes.hpp"

#include "serialization/SerializationArchive.hpp"
#include "serialization/SerializeTrait.hpp"

#include <memory>

namespace pulvis::fs
{
	using backend_factory_t = std::unique_ptr<serialization::IArchiveBackend>(*)();
//////////////////////////////////////////////////////////////////////////
	void Initialize();
	void Shutdown();

	void Mount(EDomain _domain, const CFilePath& _virtual_prefix,
		std::shared_ptr<IFileSource> _source,
		EMountTag _tag = EMountTag::Game);

	void Unmount(EDomain _domain, const CFilePath& _virtual_prefix);

	EFileResult ReadFile(EDomain _domain, const CFilePath& _path, CFileBuffer& _out_buffer);
	EFileResult WriteFile(EDomain _domain, const CFilePath& _path, const CFileBuffer& _buffer);
	bool FileExists(EDomain _domain, const CFilePath& _path);

	EFileResult ReadFile(const CFilePath& _path, CFileBuffer& _out_buffer);
	bool FileExists(const CFilePath& _path);

	CMountSystem& GetMountSystem();
//////////////////////////////////////////////////////////////////////////
	void RegisterBackend(const std::string& _file_extension, backend_factory_t _factory);
	std::unique_ptr<serialization::IArchiveBackend> CreateBackend(std::string_view _extension);

	template<typename T>
	EFileResult Save(EDomain _domain, const CFilePath& _path, const T& _data)
	{
		auto backend = CreateBackend(_path.Extension());
		if (!backend)
			return EFileResult::NoSerializer;

		serialization::CSerializationArchive archive(std::move(backend), serialization::EArchiveMode::Write);
		archive.Process("root", _data);

		CFileBuffer buffer = archive.GetResult();
		return WriteFile(_domain, _path, buffer);
	}

	template<typename T>
	EFileResult Load(EDomain _domain, const CFilePath& _path, T& _out_data)
	{
		CFileBuffer buffer;
		EFileResult result = ReadFile(_domain, _path, buffer);
		if (result != EFileResult::Success)
			return result;

		auto backend = CreateBackend(_path.Extension());
		if (!backend)
			return EFileResult::NoSerializer;

		serialization::CSerializationArchive archive(std::move(backend), serialization::EArchiveMode::Read);
		archive.SetInput(buffer);
		archive.Process("root", _out_data);
		return EFileResult::Success;
	}

	template<typename T>
	EFileResult Load(const CFilePath& _path, T& _out_data)
	{
		CFileBuffer buffer;
		EFileResult result = ReadFile(_path, buffer);
		if (result != EFileResult::Success)
			return result;

		auto backend = CreateBackend(_path.Extension());
		if (!backend)
			return EFileResult::NoSerializer;

		serialization::CSerializationArchive archive(std::move(backend), serialization::EArchiveMode::Read);
		archive.SetInput(buffer);
		archive.Process("root", _out_data);
		return EFileResult::Success;
	}
}