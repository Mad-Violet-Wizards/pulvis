#include "GLSpriteRenderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "assets/AssetRegistry.hpp"
#include "assets/AssetEntry.hpp"
#include "FilePath.hpp"
#include "FileTypes.hpp"
#include "Logger.hpp"

namespace pulvis::rendering::gl
{
	CGLSpriteRenderer::CGLSpriteRenderer(CGLRenderDevice& _render_device, pulvis::fs::assets::CAssetRegistry& _assets_registry)
		: m_Device(_render_device)
		, m_AssetsRegistry(_assets_registry)
		, m_Shader()
		, m_Vao(0)
		, m_Vbo(0)
		, m_Ibo(0)
		, m_Initialized(false)
	{
	}

	CGLSpriteRenderer::~CGLSpriteRenderer()
	{
		Shutdown();
	}

	void CGLSpriteRenderer::Initialize()
	{
		if (m_Initialized)
		{
			return;
		}

		if (!LoadAndCompileShader("engine/shaders/sprite2d_shader.vert", "engine/shaders/sprite2d_shader.frag"))
		{
			PULVIS_ERROR_LOG("GLSpriteRenderer: Shader invalid, initialization failed.");
			return;
		}

		if (!m_Shader.IsValid())
		{
			PULVIS_ERROR_LOG("GLSpriteRenderer: Shader invalid, initialization failed.");
		}

		constexpr float sprite_shader_vertices[] =
		{
			// pos			// uv
			0.f, 0.f,		0.f, 0.f,
			1.f, 0.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			0.f, 1.f,		0.f, 1.f
		};

		constexpr unsigned int sprite_shader_indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		m_Vbo = m_Device.CreateVertexBuffer(sprite_shader_vertices, sizeof(sprite_shader_vertices), EBufferUsage::Static);
		m_Ibo = m_Device.CreateIndexBuffer(sprite_shader_indices, sizeof(sprite_shader_indices), EBufferUsage::Static);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

		m_Initialized = true;
	}

	void CGLSpriteRenderer::Shutdown()
	{
		if (!m_Initialized)
		{
			return;
		}

		if (m_Ibo.ID != 0)
		{
			m_Device.DestroyBuffer(m_Ibo);
		}

		if (m_Vbo.ID != 0)
		{
			m_Device.DestroyBuffer(m_Vbo);
		}

		if (m_Vao != 0)
		{
			glDeleteVertexArrays(1, &m_Vao);
			m_Vao = 0;
		}

		m_Initialized = false;
	}

	void CGLSpriteRenderer::Draw(const CCamera2D& _camera, const SSpriteDrawCmd& _cmd, const SFrameRenderState& _frame_state)
	{
		if (!m_Initialized)
			return;

		m_Shader.Use();

		// Model = translate(pos) * scale(size)
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(_cmd.Position, 0.0f));
		model = glm::scale(model, glm::vec3(_cmd.Size, 1.0f));

		glm::mat4 mvp = _camera.GetViewProjMatrix() * model;

		m_Shader.SetMat4("uMVP", mvp);
		m_Shader.SetInt("uTexture", 0);

		m_Shader.SetVec4("uUVRect", _cmd.UVRect);

		glm::vec4 final_color = _cmd.Color * _frame_state.AmbientLight;
		m_Shader.SetVec4("uColor", final_color);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _cmd.TextureID);

		glBindVertexArray(m_Vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	bool CGLSpriteRenderer::LoadAndCompileShader(const std::string& _vert_path, const std::string& _frag_path)
	{
		using namespace pulvis::fs;
		using namespace pulvis::fs::assets;

		auto load_source = [&](const std::string& _virtual_path) -> std::string
			{
				SAssetHandle handle = m_AssetsRegistry.Find(_virtual_path);

				if (handle == INVALID_ASSET_HANDLE)
				{
					handle = m_AssetsRegistry.RegisterAsset(EDomain::Engine, _virtual_path, EAssetType::Shader);
					if (!m_AssetsRegistry.LoadAndProcess(handle))
					{
						PULVIS_ERROR_LOG("Failed to load shader '{}'.", _virtual_path);
						return {};
					}
				}

				const SAssetEntry* entry = m_AssetsRegistry.Get(handle);
				if (!entry || entry->RawData.Empty())
				{
					PULVIS_ERROR_LOG("Shader '{}' has no source data.", _virtual_path);
					return {};
				}

				return entry->RawData.ToString();
			};

		const std::string vert_source = load_source(_vert_path);
		const std::string frag_source = load_source(_frag_path);

		if (vert_source.empty() || frag_source.empty())
		{
			return false;
		}

		return m_Shader.CompileFromSource(vert_source.c_str(), frag_source.c_str());
	}
}

