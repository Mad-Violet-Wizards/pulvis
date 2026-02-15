#include "IRenderer.hpp"

namespace pulvis::rendering
{
	IRenderer::IRenderer(ERendererType _renderer_type)
		: m_RendererType(_renderer_type)
	{
	}

	ERendererType IRenderer::GetType() const
	{
		return m_RendererType;
	}
}