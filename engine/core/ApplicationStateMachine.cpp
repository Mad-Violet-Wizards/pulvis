#include "engine/engine_pch.hpp"
#include "ApplicationStateMachine.hpp"

#include "engine/core/ApplicationContext.hpp"
#include "engine/project/ProjectService.hpp"
#include "engine/rendering/RenderingService.hpp"

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

		std::unique_ptr<IApplicationState> state_project_create = std::make_unique<AppState_ProjectCreate>(this);
		m_AvailableStates.emplace_back(std::move(state_project_create));

		std::unique_ptr<IApplicationState> state_project_load = std::make_unique<AppState_ProjectLoad>(this);
		m_AvailableStates.emplace_back(std::move(state_project_load));

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

		engine::projects::ProjectService::GetInstance().Initialize(m_StateMachine->GetContext()->m_Filesystem);
		engine::rendering::RenderingService::GetInstance().Initialize(rendering::ERendererType::OpenGL);
		m_StateMachine->QueueState(EApplicationState::ProjectLoad);

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
	AppState_ProjectCreate::AppState_ProjectCreate(ApplicationStateMachine* _stateMachine)
		: IApplicationState(_stateMachine)
	{
	}

	void AppState_ProjectCreate::OnEnter()
	{
	}

	void AppState_ProjectCreate::OnExit()
	{
	}

	void AppState_ProjectCreate::Update()
	{
		if (m_ProjectName.empty())
		{
			std::string project_name;
			std::cout << "Enter project name: ";
			std::cin >> project_name;
			m_ProjectName = project_name;
		}

		if (m_ProjectPath.empty())
		{
			std::string project_path;
			std::cout << "Enter ABSOLUTE project path: ";
			std::cin >> project_path;
			m_ProjectPath = project_path;
		}

		if (!m_CreateDirectory.has_value())
		{
			std::string create_directory;
			std::cout << "Create directory? (y/n): ";
			std::cin >> create_directory;
			m_CreateDirectory = create_directory == "y";
		}
			
		if (!m_ProjectName.empty() && !m_ProjectPath.empty() && m_CreateDirectory.has_value())
		{
 			engine::projects::ProjectService::GetInstance().CreateProject(m_ProjectName, m_ProjectPath, m_CreateDirectory.value());
			m_StateMachine->QueueState(EApplicationState::ProjectLoad);
		}
	}

	EApplicationState AppState_ProjectCreate::GetState() const
	{
		return EApplicationState::ProjectCreate;
	}


	//////////////////////////////////////////////////////////////////////////
	AppState_ProjectLoad::AppState_ProjectLoad(ApplicationStateMachine* _stateMachine)
		: IApplicationState(_stateMachine)
	{

	}

	void AppState_ProjectLoad::OnEnter()
	{
		engine::projects::ProjectService::GetInstance().GetProjectNames(m_ProjectNamesList);
	}

	void AppState_ProjectLoad::OnExit()
	{
	}

	void AppState_ProjectLoad::Update()
	{
		if (m_ProjectNameToLoad.empty())
		{
			if (m_ProjectNamesList.size() < 1)
			{
				std::cout << "Empty.\n-1 to back.";
				return;
			}

			const size_t project_idx = 0;
			m_ProjectNameToLoad = m_ProjectNamesList[project_idx];
		}
		else
		{
			ProjectService& project_service = engine::projects::ProjectService::GetInstance();
			if (!project_service.GetIsProjectLoadInProgress() && project_service.GetProjectContext() == nullptr)
			{
				engine::projects::ProjectService::GetInstance().LoadProject(m_ProjectNameToLoad);
			}

			if (project_service.ConsumeProjectLoaded())
			{
				std::cout << "[AppStateMachine::ProjectLoad] Finished at frame: " << m_StateMachine->GetContext()->m_FrameNumber << "\n";
				engine::projects::ProjectService::GetInstance().SetupShaders();
				m_ProjectNamesList.clear();
				m_ProjectNameToLoad.clear();
				m_StateMachine->QueueState(EApplicationState::GameLoop);
			}
		}
	}

	EApplicationState AppState_ProjectLoad::GetState() const
	{
		return EApplicationState::ProjectLoad;
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