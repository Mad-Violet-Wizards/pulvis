#include "GameStateMachine.hpp"
#include "GameState.hpp"
#include "Assert.hpp"

namespace pulvis::game_engine
{
  CGameStateMachine::CGameStateMachine()
  {
  }
  CGameStateMachine::~CGameStateMachine()
  {
  }

  void CGameStateMachine::PushState(std::unique_ptr<IGameState> _state)
    {
        ASSERT(_state, "Cannot push null state.");

        m_StateStack.push_back(std::move(_state));
        m_StateStack.back()->OnEnter();
    }

    void CGameStateMachine::PopState()
    {
        ASSERT(!m_StateStack.empty(), "Cannot pop state from empty stack.");

        m_StateStack.back()->OnExit();
        m_StateStack.pop_back();
    }

    void CGameStateMachine::SwitchState(std::unique_ptr<IGameState> _state)
    {
        ASSERT(_state, "Cannot switch to null state.");

        if (!m_StateStack.empty())
        {
            PopState();
        }

        PushState(std::move(_state));
    }

    void CGameStateMachine::Frame(float _dt)
    {
        if (m_StateStack.empty())
        {
            return;
        }

        IGameState* current_state = m_StateStack.back().get();
        current_state->Frame(_dt);

        if (current_state->WantsExit())
        {
            PopState();
        }
    }

    void CGameStateMachine::Render()
    {
        if (m_StateStack.empty())
        {
            return;
        }

        m_StateStack.back()->Render();
    }

    bool CGameStateMachine::Empty() const
    {
        return m_StateStack.empty();
    }

    IGameState* CGameStateMachine::GetCurrentState() const
    {
        if (m_StateStack.empty())
        {
            return nullptr;
        }

        return m_StateStack.back().get();
    }
}