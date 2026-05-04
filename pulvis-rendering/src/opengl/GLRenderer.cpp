#include "IRenderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

#include "GLRenderer.hpp"
#include "GLSpriteRenderer.hpp"
#include "FileSystem.hpp"
#include "Logger.hpp"

namespace pulvis::rendering::gl
{
	CGLRenderer::CGLRenderer(pulvis::fs::assets::CAssetRegistry& _asset_registry)
		: IRenderer(ERendererType::OpenGL)
		, m_AssetRegistry(_asset_registry)
		, m_Window(800, 600, "Test")
		, m_Camera(800.f, 600.f)
		, m_SpriteRenderer(m_Device, _asset_registry)
		, m_TileRenderer(m_Device, _asset_registry)
		, m_CurrentBlend(ERenderLayerBlend::AlphaBlend)
	{
	}

	void CGLRenderer::Initialize()
	{
		glEnable(GL_BLEND);
		ApplyBlendMode(m_CurrentBlend);
	}

	void CGLRenderer::BeginFrame()
	{
		m_Device.Clear(0.f, 0.f, 0.1f, 1.f);
	}

	void CGLRenderer::Frame()
	{
		m_RenderQueue.SortAll();

		const SFrameRenderState& frame_state = m_RenderQueue.GetFrameState();

		for (SRenderLayerHandle layer : m_LayerCache.GetSortedLayers())
		{
			const SRenderLayerDesc& desc = m_LayerCache.Get(layer);

			const auto& tile_batches = m_RenderQueue.GetTilesForLayer(layer);
			const auto& sprite_draws = m_RenderQueue.GetSpritesForLayer(layer);

			if (tile_batches.empty() && sprite_draws.empty())
			{
				continue;
			}

			ApplyBlendMode(desc.Blend);

			if (!tile_batches.empty())
			{
				if (!m_TileRenderer.IsInitialized())
				{
					m_TileRenderer.Initialize();
				}

				m_TileRenderer.Draw(tile_batches, m_Camera, frame_state);
			}

			if (!sprite_draws.empty())
			{
				if (!m_SpriteRenderer.IsInitialized())
				{
					m_SpriteRenderer.Initialize();
				}

				for (const SSpriteDrawCmd& cmd : sprite_draws)
				{
					m_SpriteRenderer.Draw(m_Camera, cmd, frame_state);
				}
			}
		}

		m_RenderQueue.Clear();
	}

	void CGLRenderer::EndFrame()
	{
		glfwSwapBuffers(m_Window.GetNativeWindow());
		glfwPollEvents();
	}

	void CGLRenderer::Shutdown()
	{
		m_SpriteRenderer.Shutdown();
		m_TileRenderer.Shutdown();
		m_LayerCache.Clear();
	}

	bool CGLRenderer::GetShouldClose()
	{
		return m_Window.GetShouldClose();
	}

	void CGLRenderer::ApplyBlendMode(ERenderLayerBlend _blend)
	{
		if (_blend == m_CurrentBlend)
		{
			return;
		}
		switch (_blend)
		{
			case ERenderLayerBlend::Opaque:
				glBlendFunc(GL_ONE, GL_ZERO);
				break;
			case ERenderLayerBlend::AlphaBlend:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case ERenderLayerBlend::Additive:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case ERenderLayerBlend::Multiply:
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				break;
			default:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
		}

		m_CurrentBlend = _blend;
	}
}
