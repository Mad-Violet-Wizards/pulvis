#pragma once

#include "ProjectFileDataModel.hpp"
#include "ProjectContext.hpp"
#include "engine/threads/ThreadTask.hpp"

namespace engine::projects
{
	////////////////////////////////////////////////////////////////////////////////
	class ProjectService : public Singleton<ProjectService>
	{
		public:

			ProjectService();
			~ProjectService();

			void Initialize(engine::fs::Filesystem* _filesystem);

			void CreateProject(const std::string& _project_name, const std::string& _project_path, bool create_dir);
			void LoadProject(const std::string& _project_name);
			void GetProjectNames(std::vector<std::string>& _out_project_names) const;

			CProjectContext* const GetProjectContext() const;

			void Frame();

			bool ConsumeProjectLoaded();
			bool GetIsProjectLoadInProgress() const;

			void SetupShaders() const;
			
	private:

		void ThreadTask_LoadProject();
		void ThreadTask_UnloadProject();

		friend class Singleton<ProjectService>;

		std::vector<std::shared_ptr<CProjectFileDataModel>> m_Projects;
		std::unique_ptr<CProjectContext> m_ProjectContext{ nullptr };
		engine::threads::CThreadTask* m_LoadProjectTask{ nullptr };

		std::unordered_map<std::string, std::vector<std::shared_ptr<CShaderFileDataModel>>> m_ShaderDataModels;

		std::atomic<bool> m_ProjectLoadInProgress;
		std::atomic<bool> m_ProjectLoaded;


	};
};