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
	CGLRenderer::CGLRenderer()
		: IRenderer(ERendererType::OpenGL)
		, m_Window(800, 600, "Test")
		, m_Camera(800.f, 600.f)
		, m_SpriteRenderer(m_Device)
		, m_TestTexture(0)
	{
	}

	void CGLRenderer::Initialize()
	{
		m_SpriteRenderer.Initialize();

		SGLTextureDesc desc;

		const std::filesystem::path texture_path = pulvis::core::GetAssetsPath("container.jpg");
		unsigned char* data = stbi_load(texture_path.string().c_str(), &desc.Width, &desc.Height, &desc.Channels, 0);
		m_TestTexture = m_Device.CreateTexture2D(desc, data);
		stbi_image_free(data);
	}

	void CGLRenderer::BeginFrame()
	{
		m_Device.Clear(0.f, 0.f, 0.1f, 1.f);
	}

	void CGLRenderer::Frame()
	{
		glm::mat4 vp = m_Camera.GetViewProjMatrix();

		m_SpriteRenderer.Begin(vp);

		glm::vec2 pos = { 0.f, 0.f };
		glm::vec2 size = { 512.0f, 512.0f };

		m_SpriteRenderer.Draw(pos, size, m_TestTexture);
		m_SpriteRenderer.End();
	}

	void CGLRenderer::EndFrame()
	{
		glfwSwapBuffers(m_Window.GetNativeWindow());
		glfwPollEvents();
	}

	void CGLRenderer::Shutdown()
	{
		m_SpriteRenderer.Shutdown();
		m_Device.DestroyTexture(m_TestTexture);
	}

	bool CGLRenderer::GetShouldClose()
	{
		return m_Window.GetShouldClose();
	}
}
