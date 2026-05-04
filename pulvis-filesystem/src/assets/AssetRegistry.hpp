#pragma once

#include "AssetEntry.hpp"
#include "AssetLoader.hpp"
#include "MountSystem.hpp"

#include <deque>
#include <unordered_map>
#include <string>
#include <memory>

namespace pulvis::events { class CEventDispatcher; }

namespace pulvis::fs::assets
{
	class CAssetRegistry
	{
		public:

			explicit CAssetRegistry(CMountSystem& _mount_system, pulvis::events::CEventDispatcher& _event_dispatcher);
			~CAssetRegistry() = default;

			CAssetRegistry(const CAssetRegistry&) = delete;
			CAssetRegistry& operator=(const CAssetRegistry&) = delete;

			void RegisterLoader(EAssetType _type, std::unique_ptr<IAssetLoader> _loader);
			
			SAssetHandle RegisterAsset(EDomain _domain, const std::string& _virtual_path, EAssetType _type);
			void UnregisterAsset(SAssetHandle& _handle);

			bool Load(const SAssetHandle& _handle);
			bool Process(const SAssetHandle& _handle);
			bool LoadAndProcess(const SAssetHandle& _handle);
			bool ReloadAsset(const SAssetHandle& _handle);

			uint32_t LoadDirectory(EDomain _domain, const std::string& _virtual_directory, bool _recursive = true);
			std::vector<SAssetHandle> LoadBatch(EDomain _domain, const std::vector<std::string>& _virtual_paths);

			void SetLoadedData(const SAssetHandle& _handle, CFileBuffer _data);

			const SAssetEntry* Get(const SAssetHandle& _handle) const;
			SAssetEntry* Get(const SAssetHandle& _handle);
			SAssetHandle Find(const std::string& _virtual_path) const;

			[[nodiscard]] std::vector<SAssetHandle> EnumerateByPrefix(EAssetType _type, std::string_view _virtual_prefix) const;
			const std::deque<SAssetEntry>& GetEntries() const { return m_Entries; }

		private:

			bool IsHandleValid(const SAssetHandle& _handle) const;
			IAssetLoader* GetLoader(EAssetType _type) const;
			void LoadDirectoryRecursive(EDomain _domain, const CFilePath& _virtual_path, IFileSource* _source, const CFilePath& _local_dir, bool _recursive, uint32_t& _out_count);

		private:

			CMountSystem& m_MountSystem;
			pulvis::events::CEventDispatcher& m_EventDispatcher;
			std::deque<SAssetEntry> m_Entries;
			std::vector<uint32_t> m_FreeList;
			std::unordered_map<std::string, uint32_t> m_PathIndex;
			std::unordered_map<EAssetType, std::unique_ptr<IAssetLoader>> m_Loaders;

	};
}