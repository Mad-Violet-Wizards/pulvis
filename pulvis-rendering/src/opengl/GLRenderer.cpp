#pragma once

#include "IRenderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

#include "GLRenderer.hpp"
#include "GLSpriteRenderer.hpp"
#include "Filesystem.hpp"

namespace pulvis::rendering::gl
{
	CGLRenderer::CGLRenderer(pulvis::fs::assets::CAssetRegistry& _asset_registry)
		: IRenderer(ERendererType::OpenGL)
		, m_AssetRegistry(_asset_registry)
		, m_Window(800, 600, "Test")
		, m_Camera(800.f, 600.f)
		, m_SpriteRenderer(m_Device, _asset_registry)
		, m_TileRenderer(m_Device, _asset_registry)
	{
	}

	void CGLRenderer::Initialize()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void CGLRenderer::BeginFrame()
	{
		m_Device.Clear(0.f, 0.f, 0.1f, 1.f);
	}

	void CGLRenderer::Frame()
	{
		m_RenderQueue.SortAll();

		const SFrameRenderState& frame_state = m_RenderQueue.GetFrameState();

		const std::vector<STileDrawCmd>& tile_batches = m_RenderQueue.GetTileDraws();

		if (!tile_batches.empty())
		{
			// For now -- Lazy init
			if (!m_TileRenderer.IsInitialized())
			{
				m_TileRenderer.Initialize();
			}

			m_TileRenderer.Draw(tile_batches, m_Camera, frame_state);
		}

		const std::vector<SSpriteDrawCmd>& sprite_draws = m_RenderQueue.GetSpriteDraws();
		if (!sprite_draws.empty())
		{
			// For now -- lazy init.
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

	void CGLRenderer::EndFrame()
	{
		glfwSwapBuffers(m_Window.GetNativeWindow());
		glfwPollEvents();
	}

	void CGLRenderer::Shutdown()
	{
		m_SpriteRenderer.Shutdown();
		m_TileRenderer.Shutdown();
	}

	bool CGLRenderer::GetShouldClose()
	{
		return m_Window.GetShouldClose();
	}
}
