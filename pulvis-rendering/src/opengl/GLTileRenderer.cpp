#include "GLTileRenderer.hpp"

#include <glad/glad.h>
#include "Logger.hpp"

#include "Filesystem.hpp"

namespace pulvis::rendering::gl
{
	CGLTileRenderer::CGLTileRenderer(CGLRenderDevice& _render_device)
		: m_Device(_render_device)
		, m_Shader(pulvis::core::GetAssetsPath("shaders/tilemap.vert").string().c_str(),
							 pulvis::core::GetAssetsPath("shaders/tilemap.frag").string().c_str())
		, m_Initialized(false)
	{
	}

	CGLTileRenderer::~CGLTileRenderer()
	{
		Shutdown();
	}

	void CGLTileRenderer::Initialize()
	{
		if (m_Initialized)
		{
			return;
		}

		if (!m_Shader.IsValid())
		{
			PULVIS_ERROR_LOG("GLTileRenderer: Shader invalid, initialization failed.");
		}

		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		m_Initialized = true;
	}

	void CGLTileRenderer::Shutdown()
	{
		if (!m_Initialized || m_Vao == 0)
		{
			return;
		}

		glDeleteVertexArrays(1, &m_Vao);
		m_Vao = 0;
		m_Initialized = false;
	}

	void CGLTileRenderer::Draw(const std::vector<STileRenderBatch>& _batches,
		const CCamera2D& _camera)
	{
		if (!m_Initialized)
		{
			Initialize();
		}

		const glm::mat4 vp = _camera.GetViewProjMatrix();

		m_Shader.Use();
		m_Shader.SetMat4("uMVP", vp);

		glBindVertexArray(m_Vao);

		for (const STileRenderBatch& batch : _batches)
		{
			if (batch.BufferHandle.ID == 0 || batch.VertexCount == 0)
				continue;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, batch.TextureHandle.ID);
			m_Shader.SetInt("uTexture", 0);
			m_Shader.SetVec2("uAtlasSize", batch.TextureSize);

			glBindBuffer(GL_ARRAY_BUFFER, batch.BufferHandle.ID);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.VertexCount));
		}

		glBindVertexArray(0);
	}
}