#pragma once

#include <cstdint>

#include "GameBase.hpp"

namespace pulvis::game_engine
{
    class IGameState
    {
        public:
            
            IGameState(CGameBase& _game)
              : m_Game(_game) {}
            virtual ~IGameState() = default;

            virtual void OnEnter() = 0;
            virtual void OnExit() = 0;
            virtual void Frame(float _dt) = 0;
            virtual void Render() = 0;

            /* State can request exit. */
            virtual bool WantsExit() const = 0;

      protected:

				CGameBase& m_Game;
    };
};