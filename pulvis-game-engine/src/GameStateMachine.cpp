#include "GameStateMachine.hpp"
#include "Assert.hpp"

namespace pulvis::game_engine
{
    void CGameStateMachine::PushState(std::unique_ptr<IGameState> _state, CGameBase& _game)
    {
        ASSERT(_state, "Cannot push null state.");

        m_StateStack.push_back(std::move(_state));
        m_StateStack.back()->OnEnter(_game);
    }

    void CGameStateMachine::PopState(CGameBase& _game)
    {
        ASSERT(!m_StateStack.empty(), "Cannot pop state from empty stack.");

        m_StateStack.back()->OnExit(_game);
        m_StateStack.pop_back();
    }

    void CGameStateMachine::SwitchState(std::unique_ptr<IGameState> _state, CGameBase& _game)
    {
        ASSERT(_state, "Cannot switch to null state.");

        if (!m_StateStack.empty())
        {
            PopState(_game);
        }

        PushState(std::move(_state), _game);
    }

    void CGameStateMachine::Frame(CGameBase& _game, float _dt)
    {
        if (m_StateStack.empty())
        {
            return;
        }

        IGameState* current_state = m_StateStack.back().get();
        current_state->Frame(_game, _dt);

        if (current_state->WantsExit())
        {
            PopState(_game);
        }
    }

    void CGameStateMachine::Render(CGameBase& _game)
    {
        if (m_StateStack.empty())
        {
            return;
        }

        m_StateStack.back()->Render(_game);
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