#pragma once

#include "engine/filesystem/data_models/GameFileDataModel.hpp"
#include "engine/filesystem/data_models/ShaderFileDataModel.hpp"
#include "engine/filesystem/data_models/ScriptFileDataModel.hpp"
#include "engine/filesystem/data_models/images/PngFileDataModel.hpp"
#include "GameContext.hpp"
#include "engine/threads/ThreadTaskHandle.hpp"

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
			void ScheduleGameLoadThreadTask();
			bool IsGameLoadThreadTaskFinished() const;
			void OnGameLoadThreadTaskFinished();

			void SetupShaders() const;
			void SetupScripts();
			////////////////////////////////////////////////////////////////////////////////
			// Textures loading
			void ScheduleTexturesLoadThreadTask();
			bool IsTexturesLoadThreadTaskStarted() const;
			bool IsTexturesLoadThreadTaskFinished() const;
			void OnTexturesLoadThreadTaskFinished();

			void SetupTextures() const;

	private:

		void ThreadTask_LoadProject();
		void ThreadTask_UnloadProject();

		void ThreadTask_LoadTextures();
		void ThreadTask_UnloadTextures();

		friend class Singleton<CGameService>;

		std::unique_ptr<CGameContext> m_GameContext{ nullptr };

		std::shared_ptr<engine::threads::CThreadTaskHandle<void>> m_LoadProjectTaskHandle{ nullptr };
		std::shared_ptr<engine::threads::CThreadTaskHandle<void>> m_LoadTexturesTaskHandle{ nullptr };

		SGameLoadThreadTaskData m_GameLoadThreadTaskData;
		STexturesLoadThreadTaskData m_TexturesLoadThreadTaskData;


	};
};