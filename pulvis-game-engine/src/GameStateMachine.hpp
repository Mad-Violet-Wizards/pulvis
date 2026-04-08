#pragma once

#include <vector>
#include <memory>

namespace pulvis::game_engine
{
    class CGameBase;
    class IGameState;

    class CGameStateMachine
    {
      public:

			  CGameStateMachine();
			  ~CGameStateMachine();

			  CGameStateMachine(const CGameStateMachine&) = delete;
			  CGameStateMachine& operator=(const CGameStateMachine&) = delete;

        void PushState(std::unique_ptr<IGameState> _state);
        void PopState();
        void SwitchState(std::unique_ptr<IGameState> _state);

        void Frame(float _dt);
        void Render();

        bool Empty() const;
        IGameState* GetCurrentState() const;

      private:

        std::vector<std::unique_ptr<IGameState>> m_StateStack;
    };
}