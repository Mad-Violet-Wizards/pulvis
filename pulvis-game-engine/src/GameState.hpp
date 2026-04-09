#pragma once

#include <cstdint>

namespace pulvis::game_engine
{
    class IGame;

    class IGameState
    {
        public:
            
            IGameState(IGame& _game)
              : m_Game(_game) {}
            virtual ~IGameState() = default;

            virtual void OnEnter() = 0;
            virtual void OnExit() = 0;
            virtual void Frame(float _dt) = 0;
            virtual void Render() = 0;

            /* State can request exit. */
            virtual bool WantsExit() const = 0;

      protected:

				IGame& m_Game;
    };
};