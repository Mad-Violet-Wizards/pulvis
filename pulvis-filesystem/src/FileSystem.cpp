#include "FileSystem.hpp"

#include "serialization/JsonArchiveBackend.hpp"
#include "serialization/BinaryArchiveBackend.hpp"

namespace pulvis::fs
{
	//////////////////////////////////////////////////////////////////////////
	namespace
	{
		CMountSystem g_MountSystem;
		std::unordered_map<std::string, backend_factory_t> g_BackendSerializersRegistry;
	}

	//////////////////////////////////////////////////////////////////////////
	void Initialize()
	{
		RegisterBackend(".json", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CJsonArchiveBackend>(); });
		RegisterBackend(".bin", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
		RegisterBackend(".dat", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
		RegisterBackend(".sav", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
	}

	void Shutdown()
	{
	}

	void Mount(EDomain _domain, const CFilePath& _virtual_prefix,
		std::shared_ptr<IFileSource> _source, EMountTag _tag)
	{
		g_MountSystem.Mount(_domain, _virtual_prefix, std::move(_source), _tag);
	}

	void Unmount(EDomain _domain, const CFilePath& _virtual_prefix)
	{
		g_MountSystem.Unmount(_domain, _virtual_prefix);
	}

	EFileResult ReadFile(EDomain _domain, const CFilePath& _path, CFileBuffer& _out_buffer)
	{
		auto resolved = g_MountSystem.Resolve(_domain, _path, false);
		if (!resolved)
			return EFileResult::NotFound;

		return resolved.Source->Read(resolved.LocalPath, _out_buffer);
	}

	EFileResult WriteFile(EDomain _domain, const CFilePath& _path, const CFileBuffer& _buffer)
	{
		auto resolved = g_MountSystem.Resolve(_domain, _path, true);
		if (!resolved)
			return EFileResult::AccessDenied;

		return resolved.Source->Write(resolved.LocalPath, _buffer);
	}

	bool FileExists(EDomain _domain, const CFilePath& _path)
	{
		return static_cast<bool>(g_MountSystem.Resolve(_domain, _path, false));
	}

	EFileResult ReadFile(const CFilePath& _path, CFileBuffer& _out_buffer)
	{
		auto resolved = g_MountSystem.ResolveAny(_path);
		if (!resolved)
			return EFileResult::NotFound;

		return resolved.Source->Read(resolved.LocalPath, _out_buffer);
	}

	bool FileExists(const CFilePath& _path)
	{
		return static_cast<bool>(g_MountSystem.ResolveAny(_path));
	}

	CMountSystem& GetMountSystem()
	{
		return g_MountSystem;
	}

	//////////////////////////////////////////////////////////////////////////
	void RegisterBackend(const std::string& _file_extension, backend_factory_t _factory)
	{
		g_BackendSerializersRegistry[_file_extension] = std::move(_factory);
	}

	std::unique_ptr<serialization::IArchiveBackend> CreateBackend(std::string_view _extension)
	{
		auto it = g_BackendSerializersRegistry.find(std::string(_extension));
		if (it != g_BackendSerializersRegistry.end())
		{
			return it->second();
		}

		return nullptr;
	}
}
