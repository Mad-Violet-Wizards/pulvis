#include "engine/engine_pch.hpp"
#include "Renderer.hpp"

namespace engine::rendering
{
	IRenderer::IRenderer()
		: m_RendererType(ERendererType::Undefined)
	{
	}
}