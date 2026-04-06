#pragma once

#include "GameBase.hpp"

class CGame : public pulvis::game_engine::CGameBase
{
	public:

		CGame() = default;
		~CGame() override = default;

		void Configure(pulvis::game_engine::SEngineConfig& _config) override;
		void OnInitialize() override;
		void OnShutdown() override;
		void Frame(float _dt) override;
		void Render() override;
};