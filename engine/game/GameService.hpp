#pragma once

#include "engine/filesystem/data_models/GameFileDataModel.hpp"
#include "GameContext.hpp"
#include "engine/threads/ThreadTask.hpp"

namespace engine::game
{
	////////////////////////////////////////////////////////////////////////////////
	struct SGameLoadThreadTaskData
	{
		void Clear();

		std::unordered_map<std::string, std::vector<std::shared_ptr<CShaderFileDataModel>>> m_ShaderDataModels;
		std::unordered_map<std::string, std::shared_ptr<CScriptFileDataModel>> m_ScriptDataModels;
	};
	////////////////////////////////////////////////////////////////////////////////
	class CGameService : public Singleton<CGameService>
	{
		public:

			CGameService();
			~CGameService();

			void Initialize(engine::fs::Filesystem* _filesystem);

			CGameContext* const GetGameContext() const;

			void Frame();

			void StartGameLoadThreadTask();
			bool ConsumeProjectLoaded();
			bool GetIsProjectLoadInProgress() const;

			void SetupShaders() const;
			void SetupScripts();

	private:

		void ThreadTask_LoadProject();
		void ThreadTask_UnloadProject();

		friend class Singleton<CGameService>;

		std::unique_ptr<CGameContext> m_GameContext{ nullptr };
		engine::threads::CThreadTask* m_LoadProjectTask{ nullptr };

		SGameLoadThreadTaskData m_GameLoadThreadTaskData;

		std::atomic<bool> m_GameLoadInProgress;
		std::atomic<bool> m_GameLoadThreadTaskFinished;


	};
};