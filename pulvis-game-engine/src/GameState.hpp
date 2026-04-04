#pragma once

#include <cstdint>

namespace pulvis::game_engine
{
    class CGameBase;

    class IGameState
    {
        public:

            virtual ~IGameState() = default;

            virtual void OnEnter(CGameBase& _game) = 0;
            virtual void OnExit(CGameBase& _game) = 0;
            virtual void Frame(CGameBase& _game, float _dt) = 0;
            virtual void Render(CGameBase& _game) = 0;

            /* State can request exit. */
            virtual bool WantsExit() const = 0;
    };
};