#pragma once

#include "pipelines/RenderPipelineOpenGL.hpp"

namespace engine::rendering::opengl
{
	enum class ERenderLayerOpenGL;

	class CRenderSubLayerOpenGL
	{
	public:

		CRenderSubLayerOpenGL();
		CRenderSubLayerOpenGL(ERenderLayerOpenGL _parent_layer, unsigned int _sublayer_index);

	private:

		ERenderLayerOpenGL m_ParentLayer;
		unsigned int m_SublayerIndex;
		std::vector<engine::rendering::opengl::pipeline::IRenderPipelineOpenGL*> m_Pipelines;
	};
}