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

	void CGLTileRenderer::Draw(const std::vector<STileRenderBatch>& _batches, const CCamera2D& _camera)
	{
	}
}