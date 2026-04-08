#include "GameStatePlay.hpp"

#include "GameBase.hpp"
#include "LevelService.hpp"
#include "Level.hpp"
#include "Chunk.hpp"
#include "Logger.hpp"

CGameStatePlay::CGameStatePlay(pulvis::game_engine::CGameBase& _game)
	: IGameState(_game)
	, m_LevelReady(false)
{
}

void CGameStatePlay::OnEnter()
{
	PULVIS_INFO_LOG("[Play] Entering play state...");
	EnsureLevelLoaded();
}

void CGameStatePlay::OnExit()
{
	PULVIS_INFO_LOG("[Play] Exiting play state...");

	pulvis::level::CLevelService& level_service = m_Game.GetLevelService();

	if (level_service.HasActiveLevel())
	{
		const uint32_t saved = level_service.SaveDirtyChunks();
		PULVIS_INFO_LOG("[Play] Saved {} dirty chunks on exit.", saved);
	}
}

void CGameStatePlay::Frame(float _dt)
{
}

void CGameStatePlay::Render()
{
	// Render active level.
}

bool CGameStatePlay::WantsExit() const
{
	return false;
}

void CGameStatePlay::EnsureLevelLoaded()
{
	if (m_LevelReady)
	{
		return;
	}

	pulvis::level::CLevelService& level_service = m_Game.GetLevelService();
	if (!level_service.HasActiveLevel())
	{
		if (level_service.ChunkExistsOnDisk("game/levels/prototype", { 0, 0 }))
		{
			LoadPrototypeLevel();
		}
		else
		{
			CreatePrototypeLevel();
		}
	}
	m_LevelReady = true;
}

void CGameStatePlay::CreatePrototypeLevel()
{
	pulvis::level::CLevelService& level_service = m_Game.GetLevelService();

	level_service.CreateEmptyLevel(PROTOTYPE_LEVEL_PATH);
	pulvis::level::CLevel* level = level_service.GetActiveLevel();

	if (!level)
	{
		PULVIS_ERROR_LOG("[Play] Failed to create prototype level.");
		return;
	}

	const pulvis::level::SChunkCoord origin_coord{ 0, 0 };
	pulvis::level::SChunk* chunk = level->GetOrCreateChunk(origin_coord);

	pulvis::level::STile ground_tile{};
	ground_tile.TilesetIndex = 0;
	ground_tile.TileIndex = 1;
	ground_tile.Flags = 0;

	chunk->Fill(0, ground_tile);

	PULVIS_INFO_LOG("[Play] Prototype level created with chunk (0,0).");

	const uint32_t saved = level_service.SaveDirtyChunks();
	PULVIS_INFO_LOG("[Play] Saved {} chunks to disk.", saved);
}

void CGameStatePlay::LoadPrototypeLevel()
{
	pulvis::level::CLevelService& level_service = m_Game.GetLevelService();

	level_service.CreateEmptyLevel(PROTOTYPE_LEVEL_PATH);

	const pulvis::level::SChunkCoord origin_coord{ 0, 0 };
	const bool loaded = level_service.LoadChunkSync(origin_coord);

	if (loaded)
	{
		PULVIS_INFO_LOG("[Play] Prototype level loaded from disk.");
	}
	else
	{
		PULVIS_WARNING_LOG("[Play] Failed to load prototype level, creating fresh.");
		CreatePrototypeLevel();
	}
}