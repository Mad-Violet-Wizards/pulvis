#include "AssetRegistry.hpp"
#include "AssetEvents.hpp"
#include "EventDispatcher.hpp"

#include "Logger.hpp"


namespace pulvis::fs::assets
{
	CAssetRegistry::CAssetRegistry(CMountSystem& _mount_system, pulvis::events::CEventDispatcher& _event_dispatcher)
		: m_MountSystem(_mount_system)
		, m_EventDispatcher(_event_dispatcher)
	{
	}

	void CAssetRegistry::RegisterLoader(EAssetType _type, std::unique_ptr<IAssetLoader> _loader)
	{
		m_Loaders[_type] = std::move(_loader);
	}

	SAssetHandle CAssetRegistry::RegisterAsset(EDomain _domain, const std::string& _virtual_path, EAssetType _type)
	{
		auto existsing_handle = m_PathIndex.find(_virtual_path);

		if (existsing_handle != m_PathIndex.end())
		{
			return m_Entries[existsing_handle->second].Handle;
		}

		uint32_t index = 0;
		uint32_t generation = 1;

		if (!m_FreeList.empty())
		{
			index = m_FreeList.back();
			m_FreeList.pop_back();
			generation = m_Entries[index].Handle.Generation + 1;
		}
		else
		{
			index = static_cast<uint32_t>(m_Entries.size());
			m_Entries.emplace_back();
		}

		SAssetEntry& entry = m_Entries[index];
		entry.Handle = { index, generation };
		entry.Type = _type;
		entry.State = EAssetState::Registered;
		entry.Domain = _domain;
		entry.VirtualPath = _virtual_path;
		entry.Payload = nullptr;
		entry.RawData.Clear();
		entry.LastAccessTime = 0;
		entry.RefCount = 0;

		m_PathIndex[_virtual_path] = index;

		return entry.Handle;
	}

	void CAssetRegistry::UnregisterAsset(SAssetHandle& _handle)
	{
		if (!IsHandleValid(_handle))
		{
			return;
		}

		SAssetEntry& entry = m_Entries[_handle.Index];

		IAssetLoader* loader = GetLoader(entry.Type);
		ASSERT(loader, "Trying to unregister asset without a valid loader.");

		if (loader)
		{
			loader->Unload(entry);
		}

		m_PathIndex.erase(entry.VirtualPath);

		entry.VirtualPath.clear();
		entry.Payload = nullptr;
		entry.State = EAssetState::Unloaded;
		m_FreeList.push_back(_handle.Index);

		SAssetUnloadedEvent unloaded_event;
		unloaded_event.Handle = _handle;
		unloaded_event.VirtualPath = entry.VirtualPath;
		unloaded_event.Type = entry.Type;
		m_EventDispatcher.Submit<SAssetUnloadedEvent>(std::move(unloaded_event));
	}

	bool CAssetRegistry::Load(const SAssetHandle& _handle)
	{
		if (!IsHandleValid(_handle))
		{
			return false;
		}

		SAssetEntry& entry = m_Entries[_handle.Index];
		ASSERT(entry.State == EAssetState::Registered, "Trying to load asset that is not in Registered state.");
		entry.State = EAssetState::Loading;

		CMountSystem::SResolvedPath resolved = m_MountSystem.Resolve(entry.Domain, CFilePath(entry.VirtualPath), false);

		if (!resolved)
		{
			PULVIS_ERROR_LOG("Failed to load asset '{}': File not found.", entry.VirtualPath);
			entry.State = EAssetState::Failed;
			return false;
		}

		CFileBuffer buffer;
		EFileResult result = resolved.Source->Read(resolved.LocalPath, buffer);

		if (result != EFileResult::Success)
		{
			PULVIS_ERROR_LOG("Failed to load asset '{}': Error reading file ({}).", entry.VirtualPath, static_cast<int>(result));
			entry.State = EAssetState::Failed;
			return false;
		}

		entry.RawData = std::move(buffer);
		entry.State = EAssetState::Loaded;
		return true;
	}

	bool CAssetRegistry::Process(const SAssetHandle& _handle)
	{
		if (!IsHandleValid(_handle))
		{
			return false;
		}

		SAssetEntry& entry = m_Entries[_handle.Index];
		if (entry.State != EAssetState::Loaded)
		{
			PULVIS_ERROR_LOG("Trying to process asset '{}' that is not in Loaded state.", entry.VirtualPath);
			entry.State = EAssetState::Failed;
			return false;
		}

		IAssetLoader* loader = GetLoader(entry.Type);
		if (!loader)
		{
			PULVIS_ERROR_LOG("Trying to process asset '{}' without a valid loader.", entry.VirtualPath);
			entry.State = EAssetState::Failed;
			return false;
		}

		if (!loader->Process(entry))
		{
			PULVIS_ERROR_LOG("Failed to process asset '{}': Loader processing failed.", entry.VirtualPath);
			entry.State = EAssetState::Failed;
			return false;
		}

		entry.State = EAssetState::Ready;

		SAssetLoadedEvent loaded_event;
		loaded_event.Handle = entry.Handle;
		loaded_event.VirtualPath = entry.VirtualPath;
		loaded_event.Type = entry.Type;
		m_EventDispatcher.Submit<SAssetLoadedEvent>(std::move(loaded_event));
		return true;
	}

	bool CAssetRegistry::LoadAndProcess(const SAssetHandle& _handle)
	{
		if (!Load(_handle))
		{
			return false;
		}

		return Process(_handle);
	}

	bool CAssetRegistry::ReloadAsset(const SAssetHandle& _handle)
	{
		if (!IsHandleValid(_handle)) { return false; }
		SAssetEntry& entry = m_Entries[_handle.Index];

		IAssetLoader* loader = GetLoader(entry.Type);
		if (loader && entry.Payload) { loader->Unload(entry); }
		entry.Payload = nullptr;
		entry.RawData.Clear();
		entry.State = EAssetState::Registered;

		if (!LoadAndProcess(_handle)) { return false; }

		SAssetReloadedEvent reloaded_event;
		reloaded_event.Handle = entry.Handle;
		reloaded_event.VirtualPath = entry.VirtualPath;
		reloaded_event.Type = entry.Type;
		m_EventDispatcher.Submit<SAssetReloadedEvent>(std::move(reloaded_event));
		return true;
	}

	uint32_t CAssetRegistry::LoadDirectory(EDomain _domain, const std::string& _virtual_directory, bool _recursive)
	{
		CMountSystem::SResolvedPath resolved = m_MountSystem.Resolve(_domain, CFilePath(_virtual_directory), false);

		if (!resolved)
		{
			PULVIS_DEBUG_LOG("LoadDirectory: '{}' not present, skipping.", _virtual_directory);
			return 0;
		}

		uint32_t count = 0;
		LoadDirectoryRecursive(_domain, CFilePath(_virtual_directory), resolved.Source, resolved.LocalPath, _recursive, count);

		return count;
	}

	std::vector<SAssetHandle> CAssetRegistry::LoadBatch(EDomain _domain, const std::vector<std::string>& _virtual_paths)
	{
		std::vector<SAssetHandle> handles;
		handles.reserve(_virtual_paths.size());

		for (const std::string& path : _virtual_paths)
		{
			const EAssetType asset_type = FromExtension(CFilePath(path).Extension());

			if (asset_type == EAssetType::Unknown)
			{
				PULVIS_WARNING_LOG("Unknown extension, skipping: {}", path);
				continue;
			}

			const SAssetHandle handle = RegisterAsset(_domain, path, asset_type);

			if (!LoadAndProcess(handle))
			{
				handles.push_back(INVALID_ASSET_HANDLE);
				continue;
			}

			handles.push_back(handle);
		}

		return handles;
	}

	void CAssetRegistry::SetLoadedData(const SAssetHandle& _handle, CFileBuffer _data)
	{
		if (!IsHandleValid(_handle))
		{
			return;
		}

		SAssetEntry& entry = m_Entries[_handle.Index];
		entry.RawData = std::move(_data);
		entry.State = EAssetState::Loaded;
	}

	const SAssetEntry* CAssetRegistry::Get(const SAssetHandle& _handle) const
	{
		if (!IsHandleValid(_handle))
		{
			return nullptr;
		}

		return &m_Entries[_handle.Index];
	}

	SAssetEntry* CAssetRegistry::Get(const SAssetHandle& _handle)
	{
		if (!IsHandleValid(_handle))
		{
			return nullptr;
		}

		return &m_Entries[_handle.Index];
	}

	SAssetHandle CAssetRegistry::Find(const std::string& _virtual_path) const
	{
		auto it = m_PathIndex.find(_virtual_path);
		if (it == m_PathIndex.end())
		{
			return INVALID_ASSET_HANDLE;
		}

		return m_Entries[it->second].Handle;
	}

	std::vector<SAssetHandle> CAssetRegistry::EnumerateByPrefix(EAssetType _type, std::string_view _virtual_prefix) const
	{
		std::vector<SAssetHandle> result;
		result.reserve(16);

		for (const SAssetEntry& entry : m_Entries)
		{
			if (entry.Type != _type) { continue; }
			if (entry.State != EAssetState::Ready) { continue; }
			if (entry.VirtualPath.size() < _virtual_prefix.size()) { continue; }
			if (std::string_view(entry.VirtualPath).substr(0, _virtual_prefix.size()) != _virtual_prefix) { continue; }
			result.push_back(entry.Handle);
		}

		std::sort(result.begin(), result.end(),
			[this](const SAssetHandle& _a, const SAssetHandle& _b)
			{
				return m_Entries[_a.Index].VirtualPath < m_Entries[_b.Index].VirtualPath;
			});

		return result;
	}

	bool CAssetRegistry::IsHandleValid(const SAssetHandle& _handle) const
	{
		if (_handle.Index >= m_Entries.size())
		{
			return false;
		}

		return m_Entries[_handle.Index].Handle.Generation == _handle.Generation;
	}

	IAssetLoader* CAssetRegistry::GetLoader(EAssetType _type) const
	{
		auto it = m_Loaders.find(_type);
		if (it == m_Loaders.end())
		{
			return nullptr;
		}

		return it->second.get();
	}

	void CAssetRegistry::LoadDirectoryRecursive(EDomain _domain, const CFilePath& _virtual_path, IFileSource* _source, const CFilePath& _local_dir, bool _recursive, uint32_t& _out_count)
	{
		std::vector<SFileInfo> files;
		const EFileResult result = _source->ListDirectory(_local_dir, files);

		if (result != EFileResult::Success)
		{
			// Quiet: missing/empty subtrees are a normal condition (optional content dirs).
			PULVIS_DEBUG_LOG("LoadDirectory: cannot list '{}' ({}), skipping.", _virtual_path.Str(), static_cast<int>(result));
			return;
		}

		for (const SFileInfo& file_info : files)
		{
			if (file_info.IsDirectory)
			{
				if (_recursive)
				{
					const std::string child_virtual = _virtual_path.Str() + "/" + std::string(file_info.Path.Filename());
					LoadDirectoryRecursive(_domain, CFilePath(child_virtual), _source, file_info.Path, _recursive, _out_count);
				}

				continue;
			}

			const EAssetType asset_type = FromExtension(file_info.Path.Extension());

			if (asset_type == EAssetType::Unknown)
			{
				PULVIS_WARNING_LOG("Unknown extension, skipping: {}", file_info.Path.Str());
				continue;
			}

			if (!GetLoader(asset_type))
			{
				PULVIS_WARNING_LOG("No loader registered for asset type {}, skipping: {}", static_cast<int>(asset_type), _virtual_path.Str());
				continue;
			}

			const std::string file_virtual_path = _virtual_path.Str() + "/" + std::string(file_info.Path.Filename());
			const SAssetHandle handle = RegisterAsset(_domain, file_virtual_path, asset_type);

			if (LoadAndProcess(handle))
			{
				++_out_count;
			}
		}
	}
}