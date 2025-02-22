#include "engine/engine_pch.hpp"
#include "GameService.hpp"
#include "engine/core/Application.hpp"
#include "GameFileDataModel.hpp"
#include "engine/filesystem/TextFileDataModel.hpp"
#include "engine/filesystem/ShaderFileDataModel.hpp"
#include "engine/rendering/opengl/Shader.hpp"
#include "engine/rendering/RenderingService.hpp"
#include "engine/rendering/opengl/RendererOpenGL.hpp"

namespace engine::game
{
	CGameService::CGameService()
	{
		m_GameLoadInProgress = false;
		m_GameLoadThreadTaskFinished = false;
	}

	CGameService::~CGameService()
	{
	}

	void CGameService::Initialize(engine::fs::Filesystem* _filesystem)
	{
		std::vector<std::string> files;
		_filesystem->GetFilenamesInDirectory(".", files);

		for (const std::string& file : files)
		{
			if (file.find(".txt") != std::string::npos)
			{
				std::shared_ptr<IFileDataModel> project_data = std::make_shared<CTextFileDataModel>();
				CFileHandle project_handle = _filesystem->OpenFile(file, &project_data, EFileMode::Read).value();
				project_handle.Deserialize();
				CTextFileDataModel* text_file_data_model = std::dynamic_pointer_cast<CTextFileDataModel>(project_data).get();

				const std::string game_path = text_file_data_model->GetLines().front();
				m_GameContext.release();
				m_GameContext = std::make_unique<CGameContext>(game_path);

				project_handle.Close();
			}
		}
	}

	void CGameService::StartGameLoadThreadTask()
	{
		if (!m_GameContext)
		{
			return;
		}

		if (m_LoadProjectTask)
		{
				delete m_LoadProjectTask;
				m_LoadProjectTask = nullptr;
		}

		m_LoadProjectTask = new engine::threads::CThreadTask(this, &CGameService::ThreadTask_LoadProject);
		engine::core::Application::GetContext().m_ThreadPool.EnqueueTask(m_LoadProjectTask);
		m_GameLoadInProgress.store(true);
	}

	CGameContext* const CGameService::GetGameContext() const
	{
		return m_GameContext.get();
	}

	void CGameService::Frame()
	{
	}

	bool CGameService::ConsumeProjectLoaded()
	{
		const bool state = m_GameLoadThreadTaskFinished.load();
		m_GameLoadThreadTaskFinished.store(false);
		return state;
	}

	bool CGameService::GetIsProjectLoadInProgress() const
	{
		return m_GameLoadInProgress.load();
	}

	void CGameService::ThreadTask_LoadProject()
	{
		engine::fs::Filesystem* game_fs = m_GameContext->GetFilesystem();
		game_fs->Mount();

		std::unordered_map<std::string, std::vector<std::shared_ptr<CShaderFileDataModel>>> shader_data_models;

		for (auto it = game_fs->FileListBegin(); it != game_fs->FileListEnd(); ++it)
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
				CFileHandle shader_handle = game_fs->OpenFile(*it, &shader_data, EFileMode::Read).value();

				shader_handle.Deserialize();
				shader_handle.Close();

				m_ShaderDataModels[shader_name].push_back(std::dynamic_pointer_cast<CShaderFileDataModel>(shader_data));
				break;
			}	
			}
		}

		m_GameLoadInProgress.store(false);
		m_GameLoadThreadTaskFinished.store(true);
	}

	void CGameService::ThreadTask_UnloadProject()
	{
	}
	void CGameService::SetupShaders() const
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
