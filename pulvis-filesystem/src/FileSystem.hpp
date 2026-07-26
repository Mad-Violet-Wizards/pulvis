#pragma once

#include "MountSystem.hpp"
#include "FileBuffer.hpp"
#include "FilePath.hpp"
#include "FileTypes.hpp"
#include "DomainRoots.hpp"

#include "serialization/SerializationArchive.hpp"
#include "serialization/SerializeTrait.hpp"

#include <memory>

namespace pulvis::fs
{
	using backend_factory_t = std::unique_ptr<serialization::IArchiveBackend>(*)();

	class CFileSystem final
	{
		public:

			CFileSystem(const std::string& _app_name, const std::string& _assets_path = "");
			~CFileSystem() = default;

			void BootstrapDomains();

			void Mount(EDomain _domain, const CFilePath& _virtual_prefix, std::shared_ptr<IFileSource> _source, EMountTag _tag = EMountTag::Base);
			void Unmount(EDomain _domain, const CFilePath& _virtual_prefix);

			[[nodiscard]] EFileResult ReadFile(EDomain _domain, const CFilePath& _path, CFileBuffer& _out_buffer) const;
			[[nodiscard]] EFileResult ReadFile(const CFilePath& _path, CFileBuffer& _out_buffer) const;
			[[nodiscard]] EFileResult WriteFile(EDomain _domain, const CFilePath& _path, const CFileBuffer& _buffer) const;

			[[nodiscard]] bool FileExists(EDomain _domain, const CFilePath& _path) const;
			[[nodiscard]] bool FileExists(const CFilePath& _path) const;

			[[nodiscard]] CMountSystem& GetMountSystem();
			[[nodiscard]] const CMountSystem& GetMountSystem() const;
			[[nodiscard]] CDomainRoots& GetDomainRoots();
			[[nodiscard]] const CDomainRoots& GetDomainRoots() const;

		//////////////////////////////////////////////////////////////////////////
			void RegisterBackend(const std::string& _file_extension, backend_factory_t _factory);
			std::unique_ptr<serialization::IArchiveBackend> FindBackend(std::string_view _extension);

			template<typename T>
			EFileResult Save(EDomain _domain, const CFilePath& _path, const T& _data)
			{
				auto backend = FindBackend(_path.Extension());
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

				auto backend = FindBackend(_path.Extension());
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

				auto backend = FindBackend(_path.Extension());
				if (!backend)
					return EFileResult::NoSerializer;

				serialization::CSerializationArchive archive(std::move(backend), serialization::EArchiveMode::Read);
				archive.SetInput(buffer);
				archive.Process("root", _out_data);
				return EFileResult::Success;
			}

		private:

			void RegisterDefaultBackends();

		private:

			CDomainRoots m_DomainRoots;
			CMountSystem m_MountSystem;
			std::unordered_map<std::string, backend_factory_t> m_BackendSerializersRegistry;
	};
}