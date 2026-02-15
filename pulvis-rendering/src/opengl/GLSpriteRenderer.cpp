#include "GLSpriteRenderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Filesystem.hpp"
#include "Logger.hpp"

namespace pulvis::rendering::gl
{
	CGLSpriteRenderer::CGLSpriteRenderer(CGLRenderDevice& _render_device)
		: m_Device(_render_device)
		, m_Shader(pulvis::core::GetAssetsPath("shaders\\sprite2d_shader.vert").string().c_str(),
							 pulvis::core::GetAssetsPath("shaders\\sprite2d_shader.frag").string().c_str())
		, m_Vao(0)
		, m_Vbo(0)
		, m_Ibo(0)
		, m_ViewProj(1.f)
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

	void CGLSpriteRenderer::Begin(const glm::mat4& _viewProj)
	{
		m_ViewProj = _viewProj;
		m_Shader.Use();
	}

	void CGLSpriteRenderer::Draw(const glm::vec2& _pos, const glm::vec2& _size, SGLTextureHandle _texture)
	{
		if (!m_Initialized)
			return;

		// Model = translate(pos) * scale(size)
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(_pos, 0.0f));
		model = glm::scale(model, glm::vec3(_size, 1.0f));

		glm::mat4 mvp = m_ViewProj * model;

		m_Shader.SetMat4("uMVP", mvp);
		m_Shader.SetInt("uTexture", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture.ID);

		glBindVertexArray(m_Vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void CGLSpriteRenderer::End()
	{
	}
}

