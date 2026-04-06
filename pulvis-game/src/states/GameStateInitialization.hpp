#pragma once

#include "GameState.hpp"

class CGameStateInitialization : public pulvis::game_engine::IGameState
{
	public:

		void OnEnter(pulvis::game_engine::CGameBase& _game) override;
		void OnExit(pulvis::game_engine::CGameBase& _game) override;
		void Frame(pulvis::game_engine::CGameBase& _game, float _dt) override;
		void Render(pulvis::game_engine::CGameBase& _game) override;
		bool WantsExit() const override;

	private:

		bool m_LoadingComplete = false;
};