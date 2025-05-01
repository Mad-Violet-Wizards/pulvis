#include "engine/engine_pch.hpp"
#include "RenderLayerOpenGL.hpp"
#include "RenderSubLayerOpenGL.hpp"

namespace engine::rendering::opengl
{
  CRenderLayerOpenGL::CRenderLayerOpenGL(ERenderLayerOpenGL _layer)
		: m_Layer(_layer)
	{

	}

  CRenderLayerOpenGL::~CRenderLayerOpenGL()
  {
    for (CRenderSubLayerOpenGL* sublayer : m_Sublayers)
		{
			delete sublayer;
		}
		m_Sublayers.clear();
  }

  void CRenderLayerOpenGL::CreateSublayer(unsigned int _sublayer_index)
  {
    CRenderSubLayerOpenGL* sublayer = new CRenderSubLayerOpenGL(m_Layer, _sublayer_index);
		m_Sublayers.push_back(sublayer);
  }

  ERenderLayerOpenGL CRenderLayerOpenGL::GetLayerType() const
  {
    return m_Layer;
  }
}
