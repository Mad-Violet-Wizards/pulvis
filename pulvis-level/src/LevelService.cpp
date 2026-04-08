#include "LevelService.hpp"
#include "MessageBus.hpp"
#include "Logger.hpp"

namespace pulvis::level
{
	CLevelService::CLevelService(pulvis::fs::assets::CAssetRegistry& _asset_registry, pulvis::fs::CMountSystem& _mount_system)
		: m_AssetRegistry(_asset_registry)
		, m_MountSystem(_mount_system)
	{
	}

	void CLevelService::Initialize(pulvis::threads::CMessageBus& _messageBus, uint32_t _ioChannelId, uint32_t _mainChannelId)
	{
		m_ChunkIO = std::make_unique<CChunkIO>(m_MountSystem, _messageBus, _ioChannelId, _mainChannelId);
		m_ChunkIO->RegisterHandlers();

		PULVIS_INFO_LOG("LevelService initialized.");
	}

	void CLevelService::Shutdown()
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			const uint32_t saved = SaveDirtyChunks();
			if (saved > 0)
			{
				PULVIS_INFO_LOG("LevelService::Shutdown - Saved {} dirty chunks.", saved);
			}
		}

		m_ActiveLevel.reset();
		m_ChunkIO.reset();
		m_ActiveLevelPath.clear();

		PULVIS_INFO_LOG("LevelService shut down.");
	}

	void CLevelService::CreateEmptyLevel(const std::string& _levelPath)
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			SaveDirtyChunks();
		}

		m_ActiveLevel = std::make_unique<CLevel>();
		m_ActiveLevelPath = _levelPath;

		PULVIS_INFO_LOG("Empty level created: {}", _levelPath);
	}

	void CLevelService::SetActiveLevel(std::unique_ptr<CLevel> _level, const std::string& _levelPath)
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			SaveDirtyChunks();
		}

		m_ActiveLevel = std::move(_level);
		m_ActiveLevelPath = _levelPath;
	}

	CLevel* CLevelService::GetActiveLevel()
	{
		return m_ActiveLevel.get();
	}

	const CLevel* CLevelService::GetActiveLevel() const
	{
		return m_ActiveLevel.get();
	}

	bool CLevelService::LoadChunkSync(const SChunkCoord& _coord)
	{
		if (!m_ActiveLevel || !m_ChunkIO)
		{
			return false;
		}

		if (m_ActiveLevel->HasChunk(_coord))
		{
			return true;
		}

		SChunk* chunk = m_ActiveLevel->GetOrCreateChunk(_coord);
		return m_ChunkIO->LoadChunkSync(m_ActiveLevelPath, _coord, *chunk);
	}

	bool CLevelService::SaveChunkSync(const SChunk& _chunk)
	{
		if (!m_ChunkIO)
		{
			return false;
		}

		return m_ChunkIO->SaveChunkSync(m_ActiveLevelPath, _chunk);
	}

	uint32_t CLevelService::LoadChunkBatchSync(const std::vector<SChunkCoord>& _coords)
	{
		if (!m_ActiveLevel || !m_ChunkIO)
		{
			return 0;
		}

		return m_ChunkIO->LoadChunkBatchSync(m_ActiveLevelPath, _coords, *m_ActiveLevel);
	}

	uint32_t CLevelService::SaveDirtyChunks()
	{
		if (!m_ActiveLevel || !m_ChunkIO)
		{
			return 0;
		}

		return m_ChunkIO->SaveDirtyChunksSync(m_ActiveLevelPath, *m_ActiveLevel);
	}

	bool CLevelService::ChunkExistsOnDisk(const std::string& levelPath, const SChunkCoord& _coord) const
	{
		if (!m_ChunkIO)
		{
			return false;
		}

		return m_ChunkIO->ChunkExistsOnDisk(levelPath, _coord);
	}

	void CLevelService::RequestLoadChunk(const SChunkCoord& _coord)
	{
		if (!m_ChunkIO)
		{
			return;
		}

		m_ChunkIO->RequestLoadChunk(m_ActiveLevelPath, _coord);
	}

	void CLevelService::RequestLoadChunks(const std::vector<SChunkCoord>& _coords)
	{
		if (!m_ChunkIO)
		{
			return;
		}

		m_ChunkIO->RequestLoadChunks(m_ActiveLevelPath, _coords);
	}

	uint32_t CLevelService::ApplyPendingChunks()
	{
		if (!m_ActiveLevel || !m_ChunkIO)
		{
			return 0;
		}

		return m_ChunkIO->ApplyPendingChunks(*m_ActiveLevel);
	}

	size_t CLevelService::PendingChunkCount() const
	{
		if (!m_ChunkIO)
		{
			return 0;
		}

		return m_ChunkIO->PendingCount();
	}
}