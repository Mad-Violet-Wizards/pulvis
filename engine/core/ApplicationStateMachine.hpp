#pragma once

#include "engine/rtti/RTTIMacros.hpp"
#include "engine/rtti/RTTIEnum.hpp"

namespace engine::core
{
	//////////////////////////////////////////////////////////////////////////
	enum class EApplicationState
	{
		None,
		Initialize,
		ProjectCreate,
		ProjectLoad,
		GameLoad,
		GameLoop,
	};
	//////////////////////////////////////////////////////////////////////////
	class ApplicationStateMachine;
	class CApplicationContext;

	class IApplicationState
	{
		public:

			IApplicationState(ApplicationStateMachine* _stateMachine);
			~IApplicationState() = default;

			virtual void OnEnter() = 0;
			virtual void OnExit() = 0;
			virtual void Update() = 0;
			virtual EApplicationState GetState() const = 0;

		protected:

			ApplicationStateMachine* m_StateMachine;
	};

	//////////////////////////////////////////////////////////////////////////
	class AppState_Initialize : public IApplicationState
	{
		public:

			AppState_Initialize(ApplicationStateMachine* _stateMachine);
			void OnEnter() override;
			void OnExit() override;
			void Update() override;
			EApplicationState GetState() const override;
	};

	//////////////////////////////////////////////////////////////////////////
	class AppState_ProjectCreate : public IApplicationState
	{
		public:

			AppState_ProjectCreate(ApplicationStateMachine* _stateMachine);

			void OnEnter() override;
			void OnExit() override;
			void Update() override;
			EApplicationState GetState() const override;

	private:

		std::string m_ProjectName;
		std::string m_ProjectPath;
		std::optional<bool> m_CreateDirectory;
	};

	//////////////////////////////////////////////////////////////////////////
	class AppState_ProjectLoad : public IApplicationState
	{
		public:

			AppState_ProjectLoad(ApplicationStateMachine* _stateMachine);

			void OnEnter() override;
			void OnExit() override;
			void Update() override;
			EApplicationState GetState() const override;

		private:

			std::vector<std::string> m_ProjectNamesList;
			std::string m_ProjectNameToLoad;
	};

	//////////////////////////////////////////////////////////////////////////
	class AppState_GameLoop : public IApplicationState
	{
	public:

		AppState_GameLoop(ApplicationStateMachine* _stateMachine);

		void OnEnter() override;
		void OnExit() override;
		void Update() override;
		EApplicationState GetState() const override;
	};


	class ApplicationStateMachine
	{
		public:

			ApplicationStateMachine();

			void Frame();

			void SetState(EApplicationState _state);
			void QueueState(EApplicationState _state);
			EApplicationState GetState() const;

			void SetContext(CApplicationContext* _context);
			CApplicationContext* GetContext() const;

		private:

			IApplicationState* m_CurrentState;
			IApplicationState* m_QueuedState;
			std::vector<std::unique_ptr<IApplicationState>> m_AvailableStates;
			CApplicationContext* m_AppContext;
	};
}
