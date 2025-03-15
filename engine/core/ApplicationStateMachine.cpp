#include "engine/engine_pch.hpp"
#include "ApplicationStateMachine.hpp"

#include "engine/core/ApplicationContext.hpp"
#include "engine/game/GameService.hpp"
#include "engine/rendering/RenderingService.hpp"
#include "engine/scriptable/ScriptableService.hpp"
#include "engine/resources/TilesScriptable.hpp"
#include "engine/resources/ResourceService.hpp"

namespace engine::core
{
	RTTI_ENUM_API(EApplicationState);

	IApplicationState::IApplicationState(ApplicationStateMachine* _stateMachine)
		: m_StateMachine(_stateMachine)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ApplicationStateMachine::ApplicationStateMachine()
	{
		m_CurrentState = nullptr;
		m_QueuedState = nullptr;
		m_AppContext = nullptr;

		std::unique_ptr<IApplicationState> state_initialize = std::make_unique<AppState_Initialize>(this);
		m_AvailableStates.emplace_back(std::move(state_initialize));

		std::unique_ptr<IApplicationState> state_game_load = std::make_unique<AppState_GameLoad>(this);
		m_AvailableStates.emplace_back(std::move(state_game_load));

		std::unique_ptr<IApplicationState> state_game_loop = std::make_unique<AppState_GameLoop>(this);
		m_AvailableStates.emplace_back(std::move(state_game_loop));
	}

	void ApplicationStateMachine::Frame()
	{
		if (m_QueuedState)
		{
			m_CurrentState = m_QueuedState;
			m_CurrentState->OnEnter();
			m_QueuedState = nullptr;
		}

		if (m_CurrentState)
		{
			m_CurrentState->Update();
		}
	}

	void ApplicationStateMachine::SetState(EApplicationState _state)
	{
		if (m_CurrentState)
		{
			m_CurrentState->OnExit();
		}

		for (auto& state : m_AvailableStates)
		{
			if (state->GetState() == _state)
			{
				m_CurrentState = state.get();
				m_CurrentState->OnEnter();
				break;
			}
		}
	}

	void ApplicationStateMachine::QueueState(EApplicationState _state)
	{
		if (m_CurrentState)
		{
			m_CurrentState->OnExit();
		}

		for (auto& state : m_AvailableStates)
		{
			if (state->GetState() == _state)
			{
				m_QueuedState = state.get();
				break;
			}
		}
	}

	EApplicationState ApplicationStateMachine::GetState() const
	{
		if (m_CurrentState)
		{
			return m_CurrentState->GetState();
		}

		return EApplicationState::None;
	}

	void ApplicationStateMachine::SetContext(CApplicationContext* _context)
	{
		m_AppContext = _context;
	}

	CApplicationContext* ApplicationStateMachine::GetContext() const
	{
		return m_AppContext;
	}
	//////////////////////////////////////////////////////////////////////////


	AppState_Initialize::AppState_Initialize(ApplicationStateMachine* _stateMachine)
		: IApplicationState(_stateMachine)
	{
	}

	void AppState_Initialize::OnEnter()
	{
		m_StateMachine->GetContext()->m_Filesystem->Mount();

		engine::game::CGameService::GetInstance().Initialize(m_StateMachine->GetContext()->m_Filesystem);

		if (m_StateMachine->GetContext()->m_Setup.m_ClientApp != EClientApp::Playground)
		{
			engine::rendering::RenderingService::GetInstance().Initialize(rendering::ERendererType::OpenGL);
		}
		engine::scriptable::CScriptableService::GetInstance().Initialize();
		m_StateMachine->QueueState(EApplicationState::GameLoad);

	}

	void AppState_Initialize::OnExit()
	{
	}

	void AppState_Initialize::Update()
	{
	}

	EApplicationState AppState_Initialize::GetState() const
	{
		return EApplicationState::Initialize;
	}

	//////////////////////////////////////////////////////////////////////////
	AppState_GameLoad::AppState_GameLoad(ApplicationStateMachine* _stateMachine)
		: IApplicationState(_stateMachine)
		, m_ProjectLoaded(false)
		, m_TexturesLoaded(false)
	{
	}

	void AppState_GameLoad::OnEnter()
	{
		engine::game::CGameService::GetInstance().StartGameLoadThreadTask();
	}

	void AppState_GameLoad::OnExit()
	{
	}

	void AppState_GameLoad::Update()
	{
		if (engine::game::CGameService::GetInstance().ConsumeProjectLoaded())
		{
			engine::game::CGameService::GetInstance().SetupShaders();
			engine::game::CGameService::GetInstance().SetupScripts();
			engine::resources::CResourceService::GetInstance().LoadTileDefinitions();
			engine::game::CGameService::GetInstance().StartTexturesLoadThreadTask();

			m_ProjectLoaded = true;
		}

		if (engine::game::CGameService::GetInstance().ConsumeTexturesLoaded())
		{
			m_TexturesLoaded = true;
		}

		if (m_ProjectLoaded && m_TexturesLoaded)
		{
			m_StateMachine->QueueState(EApplicationState::GameLoop);
		}
	}

	EApplicationState AppState_GameLoad::GetState() const
	{
		return EApplicationState::GameLoad;
	}

	//////////////////////////////////////////////////////////////////////////
	AppState_GameLoop::AppState_GameLoop(ApplicationStateMachine* _stateMachine)
		: IApplicationState(_stateMachine)
	{
	}

	void AppState_GameLoop::OnEnter()
	{
	}

	void AppState_GameLoop::OnExit()
	{
	}

	void AppState_GameLoop::Update()
	{
	}

	EApplicationState AppState_GameLoop::GetState() const
	{
		return EApplicationState::GameLoop;
	}
}