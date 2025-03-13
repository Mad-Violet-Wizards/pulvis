#include "engine/engine_pch.hpp"
#include "GameContext.hpp"

namespace engine::game
{
	CGameContext::CGameContext(const std::string& _game_path)
	{
		m_Filesystem = new engine::fs::Filesystem("game", _game_path);
	}

	CGameContext::~CGameContext()
	{
		delete m_Filesystem;
	}

}