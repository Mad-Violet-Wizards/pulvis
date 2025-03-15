#pragma once

#include "engine/filesystem/data_models/GameFileDataModel.hpp"
#include "engine/filesystem/data_models/ShaderFileDataModel.hpp"
#include "engine/filesystem/data_models/ScriptFileDataModel.hpp"
#include "engine/filesystem/data_models/images/PngFileDataModel.hpp"
#include "GameContext.hpp"
#include "engine/threads/ThreadTask.hpp"

namespace engine::resources
{
	enum class ETileType;
	class ITile;
	class CTile;
	class CAtlasTile;
}

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
	struct STexturesLoadThreadTaskData
	{
		void Clear();
		std::unordered_map<std::string, std::shared_ptr<CPngFileDataModel>> m_PngTextureFileDataModels;
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

			////////////////////////////////////////////////////////////////////////////////
			// Initial game loading
			void StartGameLoadThreadTask();
			bool ConsumeProjectLoaded();
			bool GetIsProjectLoadInProgress() const;

			void SetupShaders() const;
			void SetupScripts();
			////////////////////////////////////////////////////////////////////////////////
			// Textures loading
			void StartTexturesLoadThreadTask();
			bool ConsumeTexturesLoaded();
			bool GetIsTexturesLoadInProgress() const;

			void SetupTextures() const;

	private:

		void ThreadTask_LoadProject();
		void ThreadTask_UnloadProject();

		void ThreadTask_LoadTextures();
		void ThreadTask_UnloadTextures();

		friend class Singleton<CGameService>;

		std::unique_ptr<CGameContext> m_GameContext{ nullptr };
		engine::threads::CThreadTask* m_LoadProjectTask{ nullptr };
		engine::threads::CThreadTask* m_LoadTexturesTask{ nullptr };

		SGameLoadThreadTaskData m_GameLoadThreadTaskData;
		STexturesLoadThreadTaskData m_TexturesLoadThreadTaskData;

		std::atomic<bool> m_GameLoadInProgress;
		std::atomic<bool> m_GameLoadThreadTaskFinished;

		std::atomic<bool> m_TexturesLoadInProgress;
		std::atomic<bool> m_TexturesLoadThreadTaskFinished;


	};
};