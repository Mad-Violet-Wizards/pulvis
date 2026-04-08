#pragma once

#include "GameState.hpp"

class CGameStateInitialization : public pulvis::game_engine::IGameState
{
	public:

		CGameStateInitialization(pulvis::game_engine::CGameBase& _game);
		void OnEnter() override;
		void OnExit() override;
		void Frame(float _dt) override;
		void Render() override;
		bool WantsExit() const override;

	private:

		bool m_LoadingComplete = false;
};