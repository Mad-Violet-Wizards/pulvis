#pragma once

#include "GameState.hpp"

class CGameStatePlay : public pulvis::game_engine::IGameState
{
	public:

		CGameStatePlay(pulvis::game_engine::CGameBase& _game);
		void OnEnter() override;
		void OnExit() override;
		void Frame(float _dt) override;
		void Render() override;
		bool WantsExit() const override;
	private:

		static constexpr const char* PROTOTYPE_LEVEL_PATH = "game/levels/prototype";

		void EnsureLevelLoaded();
		void CreatePrototypeLevel();
		void LoadPrototypeLevel();

		bool m_LevelReady;
};