#include "engine/engine_pch.hpp"
#include "ProjectService.hpp"
#include "engine/core/Application.hpp"
#include "ProjectFileDataModel.hpp"
#include "engine/filesystem/TextFileDataModel.hpp"
#include "engine/filesystem/ShaderFileDataModel.hpp"
#include "engine/rendering/opengl/Shader.hpp"
#include "engine/rendering/RenderingService.hpp"
#include "engine/rendering/opengl/RendererOpenGL.hpp"

namespace engine::projects
{
	ProjectService::ProjectService()
	{
		m_ProjectLoadInProgress = false;
		m_ProjectLoaded = false;
	}

	ProjectService::~ProjectService()
	{
	}

	void ProjectService::Initialize(engine::fs::Filesystem* _filesystem)
	{
		std::vector<std::string> project_files;
		_filesystem->GetFilenamesInDirectory(".", project_files);

		for (const std::string& project_file : project_files)
		{
			if (project_file.find(".json") != std::string::npos)
			{
				std::shared_ptr<IFileDataModel> project_data = std::make_shared<CProjectFileDataModel>();
				CFileHandle project_handle = _filesystem->OpenFile(project_file, &project_data, EFileMode::Read).value();
				project_handle.Deserialize();
				project_handle.Close();
				m_Projects.push_back(std::dynamic_pointer_cast<CProjectFileDataModel>(project_data));
			}
		}
	}

	void ProjectService::CreateProject(const std::string& _project_name, const std::string& _project_path, bool _create_dir)
	{
		std::filesystem::path project_path = _project_path;

		if (_create_dir)
		{
			project_path /= _project_name;
			std::filesystem::create_directory(project_path);
		}

		engine::fs::Filesystem* engine_fs = engine::core::Application::GetContext().m_Filesystem;
		std::shared_ptr<engine::fs::IFileDataModel> project_file_model = std::make_shared<CProjectFileDataModel>(_project_name, project_path.string(), 0, 0, 0, 0);
		
		const std::string& project_filename = _project_name + ".json";
		CFileHandle project_file_handle = engine_fs->OpenFile(project_filename, &project_file_model, EFileMode::Write).value();
		project_file_handle.Serialize();
		project_file_handle.Close();
	}

	void ProjectService::LoadProject(const std::string& _project_name)
	{
		if (m_ProjectContext)
		{
			ThreadTask_UnloadProject();
		}

 		for (const auto& project : m_Projects)
		{
			if (project->GetName() == _project_name)
			{
				m_ProjectContext.release();
				m_ProjectContext = std::make_unique<CProjectContext>(project.get());

				if (m_LoadProjectTask)
				{
					delete m_LoadProjectTask;
					m_LoadProjectTask = nullptr;
				}

				std::cout << "[ProjectService::LoadProject] Started at frame: " << engine::core::Application::GetContext().m_FrameNumber << "\n";
				m_LoadProjectTask = new engine::threads::CThreadTask(this, &ProjectService::ThreadTask_LoadProject);
				engine::core::Application::GetContext().m_ThreadPool.EnqueueTask(m_LoadProjectTask);
				m_ProjectLoadInProgress.store(true);
				break;
			}
		}
	}

	void ProjectService::GetProjectNames(std::vector<std::string>& _out_project_names) const
	{
		for (const auto& project : m_Projects)
		{
			_out_project_names.push_back(project->GetName());
		}
	}

	CProjectContext* const ProjectService::GetProjectContext() const
	{
		return m_ProjectContext.get();
	}

	void ProjectService::Frame()
	{
	}

	bool ProjectService::ConsumeProjectLoaded()
	{
		const bool state = m_ProjectLoaded.load();
		m_ProjectLoaded.store(false);
		return state;
	}

	bool ProjectService::GetIsProjectLoadInProgress() const
	{
		return m_ProjectLoadInProgress.load();
	}

	void ProjectService::ThreadTask_LoadProject()
	{
		engine::fs::Filesystem* project_fs = m_ProjectContext->GetFilesystem();
		project_fs->Mount();

		std::unordered_map<std::string, std::vector<std::shared_ptr<CShaderFileDataModel>>> shader_data_models;

		for (auto it = project_fs->FileListBegin(); it != project_fs->FileListEnd(); ++it)
		{
			const std::string file_extension = engine::fs::Filesystem::GetFileExtension(*it);
			const EFileDataModelType file_type = engine::fs::FromFileExtension(file_extension);

			switch (file_type)
			{
			case EFileDataModelType::Shader:
			{
				const engine::rendering::opengl::EShaderType shader_type = engine::rendering::opengl::FromFileExtension(file_extension);
				const std::string shader_name = [it]()
				{
						const std::string filename = engine::fs::Filesystem::GetFilename(*it);
						const std::string shader_name = filename.substr(0, filename.find_last_of('.'));
						return shader_name;
				}();

				std::shared_ptr<IFileDataModel> shader_data = std::make_shared<CShaderFileDataModel>(shader_type);
				CFileHandle shader_handle = project_fs->OpenFile(*it, &shader_data, EFileMode::Read).value();

				shader_handle.Deserialize();
				shader_handle.Close();

				m_ShaderDataModels[shader_name].push_back(std::dynamic_pointer_cast<CShaderFileDataModel>(shader_data));
				break;
			}	
			}
		}

		m_ProjectLoadInProgress.store(false);
		m_ProjectLoaded.store(true);
	}

	void ProjectService::ThreadTask_UnloadProject()
	{
	}
	void ProjectService::SetupShaders() const
	{
		auto find_shader_model = [](const std::vector<std::shared_ptr<CShaderFileDataModel>>& _shaders_file_models_vec, engine::rendering::opengl::EShaderType _shader_type) -> CShaderFileDataModel*
			{
				for (const auto& shader_file_model : _shaders_file_models_vec)
				{
					if (shader_file_model->GetShaderType() == _shader_type)
					{
						return shader_file_model.get();
					}
				}

				return nullptr;
			};

		std::vector<engine::rendering::opengl::CShader*> shaders;
		for (const auto& [shader_name, shader_file_model_vec] : m_ShaderDataModels)
		{
			engine::rendering::opengl::CShader* shader = new engine::rendering::opengl::CShader(shader_name);

			CShaderFileDataModel* vertex_shader = find_shader_model(shader_file_model_vec, engine::rendering::opengl::EShaderType::Vertex);
			CShaderFileDataModel* fragment_shader = find_shader_model(shader_file_model_vec, engine::rendering::opengl::EShaderType::Fragment);
			CShaderFileDataModel* geometry_shader = find_shader_model(shader_file_model_vec, engine::rendering::opengl::EShaderType::Geometry);

			if (vertex_shader && fragment_shader && geometry_shader)
			{
				shader->LoadShader(vertex_shader, fragment_shader, geometry_shader);
			}
			else if (vertex_shader && fragment_shader)
			{
				shader->LoadShader(vertex_shader, fragment_shader);
			}

			shaders.push_back(shader);
		}

		engine::rendering::opengl::RendererOpenGL* opengl_renderer = static_cast<engine::rendering::opengl::RendererOpenGL*>(engine::rendering::RenderingService::GetInstance().GetRenderer());
		opengl_renderer->SetupShaders(shaders);
	}
};
