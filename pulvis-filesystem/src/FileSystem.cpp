#include "FileSystem.hpp"

#include "serialization/JsonArchiveBackend.hpp"
#include "serialization/BinaryArchiveBackend.hpp"

namespace pulvis::fs
{
	CFileSystem::CFileSystem(const std::string& _app_name, const std::string& _assets_path /* = "" */)
		: m_DomainRoots(_app_name, _assets_path)
	{
		RegisterDefaultBackends();
	}

	void CFileSystem::BootstrapDomains()
	{
		m_MountSystem.BootstrapDomains(m_DomainRoots);
	}

	void CFileSystem::Mount(EDomain _domain, const CFilePath& _virtual_prefix, std::shared_ptr<IFileSource> _source, EMountTag _tag)
	{
		m_MountSystem.Mount(_domain, _virtual_prefix, std::move(_source), _tag);
	}

	void CFileSystem::Unmount(EDomain _domain, const CFilePath& _virtual_prefix)
	{
		m_MountSystem.Unmount(_domain, _virtual_prefix);
	}

	EFileResult CFileSystem::ReadFile(EDomain _domain, const CFilePath& _path, CFileBuffer& _out_buffer) const
	{
		const CMountSystem::SResolvedPath resolved = m_MountSystem.Resolve(_domain, _path, false);
		if (!resolved)
			return EFileResult::NotFound;

		return resolved.Source->Read(resolved.LocalPath, _out_buffer);
	}

	EFileResult CFileSystem::ReadFile(const CFilePath& _path, CFileBuffer& _out_buffer) const
	{
		const CMountSystem::SResolvedPath resolved = m_MountSystem.ResolveAny(_path);
		if (!resolved)
			return EFileResult::NotFound;

		return resolved.Source->Read(resolved.LocalPath, _out_buffer);
	}

	EFileResult CFileSystem::WriteFile(EDomain _domain, const CFilePath& _path, const CFileBuffer& _buffer) const
	{
		const CMountSystem::SResolvedPath resolved = m_MountSystem.Resolve(_domain, _path, true);
		if (!resolved)
		{
			return EFileResult::AccessDenied;
		}

		return resolved.Source->Write(resolved.LocalPath, _buffer);
	}

	bool CFileSystem::FileExists(EDomain _domain, const CFilePath& _path) const
	{
		return static_cast<bool>(m_MountSystem.Resolve(_domain, _path, false));
	}

	bool CFileSystem::FileExists(const CFilePath& _path) const
	{
		return static_cast<bool>(m_MountSystem.ResolveAny(_path));
	}

	CMountSystem& CFileSystem::GetMountSystem()
	{
		return m_MountSystem;
	}

	const CMountSystem& CFileSystem::GetMountSystem() const
	{
		return m_MountSystem;
	}

	CDomainRoots& CFileSystem::GetDomainRoots()
	{
		return m_DomainRoots;
	}

	const CDomainRoots& CFileSystem::GetDomainRoots() const
	{
		return m_DomainRoots;
	}

	//////////////////////////////////////////////////////////////////////////
	void CFileSystem::RegisterBackend(const std::string& _file_extension, backend_factory_t _factory)
	{
		m_BackendSerializersRegistry[_file_extension] = std::move(_factory);
	}

	std::unique_ptr<serialization::IArchiveBackend> CFileSystem::FindBackend(std::string_view _extension)
	{
		auto it = m_BackendSerializersRegistry.find(std::string(_extension));
		if (it != m_BackendSerializersRegistry.end())
		{
			return it->second();
		}

		return nullptr;
	}

	void CFileSystem::RegisterDefaultBackends()
	{
		RegisterBackend(".json", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CJsonArchiveBackend>(); });
		RegisterBackend(".bin", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
		RegisterBackend(".dat", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
		RegisterBackend(".sav", +[]() -> std::unique_ptr<serialization::IArchiveBackend> { return std::make_unique<serialization::CBinaryArchiveBackend>(); });
	}
}
