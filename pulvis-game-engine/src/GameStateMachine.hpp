#pragma once

#include "GameState.hpp"

#include <vector>
#include <memory>

namespace pulvis::game_engine
{
    class CGameBase;

    class CGameStateMachine
    {
        public:

			CGameStateMachine() = default;
			~CGameStateMachine() = default;

			CGameStateMachine(const CGameStateMachine&) = delete;
			CGameStateMachine& operator=(const CGameStateMachine&) = delete;

            void PushState(std::unique_ptr<IGameState> _state, CGameBase& _game);
            void PopState(CGameBase& _game);
            void SwitchState(std::unique_ptr<IGameState> _state, CGameBase& _game);

            void Frame(CGameBase& _game, float _dt);
            void Render(CGameBase& _game);

            bool Empty() const;
            IGameState* GetCurrentState() const;

        private:

            std::vector<std::unique_ptr<IGameState>> m_StateStack;
    };
}