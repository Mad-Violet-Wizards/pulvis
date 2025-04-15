#include "engine/engine_pch.hpp"
#include "RenderLayerOpenGL.hpp"
#include "RenderSubLayerOpenGL.hpp"

namespace engine::rendering::opengl
{
	CRenderSubLayerOpenGL::CRenderSubLayerOpenGL()
		: m_ParentLayer(ERenderLayerOpenGL::Unknown)
		, m_SublayerIndex(std::numeric_limits<unsigned int>::max())
	{
	}

	CRenderSubLayerOpenGL::CRenderSubLayerOpenGL(ERenderLayerOpenGL _parent_layer, unsigned int _sublayer_index)
		: m_ParentLayer(_parent_layer) 
		, m_SublayerIndex(_sublayer_index)
	{
	}

}