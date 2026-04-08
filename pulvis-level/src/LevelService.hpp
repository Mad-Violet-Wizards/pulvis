#pragma once

#include "Level.hpp"
#include "ChunkIO.hpp"

#include <memory>
#include <string>

namespace pulvis::fs
{
	class CMountSystem;

	namespace assets
	{
		class CAssetRegistry;
	}
}

namespace pulvis::threads { class CMessageBus; }

namespace pulvis::level
{
	class CLevelService
	{
		public:

			CLevelService(pulvis::fs::assets::CAssetRegistry& _asset_registry, pulvis::fs::CMountSystem& _mount_system);
			~CLevelService() = default;

			CLevelService(const CLevelService&) = delete;
			CLevelService& operator=(const CLevelService&) = delete;

			void Initialize(pulvis::threads::CMessageBus& _messageBus, uint32_t _ioChannelId, uint32_t _mainChannelId);
			void Shutdown();

			void CreateEmptyLevel(const std::string& _levelPath);
			void SetActiveLevel(std::unique_ptr<CLevel> _level, const std::string& _levelPath);

			CLevel* GetActiveLevel();
			const CLevel* GetActiveLevel() const;
			bool HasActiveLevel() const { return m_ActiveLevel != nullptr; }
			const std::string& GetActiveLevelPath() const { return m_ActiveLevelPath; }

			bool LoadChunkSync(const SChunkCoord& _coord);
			bool SaveChunkSync(const SChunk& _coord);
			uint32_t LoadChunkBatchSync(const std::vector<SChunkCoord>& _coords);
			uint32_t SaveDirtyChunks();
			bool ChunkExistsOnDisk(const std::string& levelPath, const SChunkCoord& _coord) const;

			void RequestLoadChunk(const SChunkCoord& _coord);
			void RequestLoadChunks(const std::vector<SChunkCoord>& _coords);
			uint32_t ApplyPendingChunks();
			size_t PendingChunkCount() const;

			CChunkIO* GetChunkIO() { return m_ChunkIO.get(); }
			const CChunkIO* GetChunkIO() const { return m_ChunkIO.get(); }

		private:

			pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
			pulvis::fs::CMountSystem& m_MountSystem;

			std::unique_ptr<CLevel> m_ActiveLevel;
			std::string m_ActiveLevelPath;
			std::unique_ptr<CChunkIO> m_ChunkIO;
	};
}