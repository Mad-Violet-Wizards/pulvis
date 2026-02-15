#pragma once

#include "Window.hpp"
#include "Camera2D.hpp"
#include "GLShader.hpp"
#include "GLRenderDevice.hpp"
#include "opengl/GLSpriteRenderer.hpp"

namespace pulvis::rendering::gl
{
	class CGLRenderer : public IRenderer
	{
		public:

			CGLRenderer();

			void Initialize() override;
			void BeginFrame() override;
			void Frame() override;
			void EndFrame() override;
			void Shutdown() override;
			bool GetShouldClose() override;

		private:

			CWindow m_Window;
			CGLRenderDevice m_Device;
			CCamera2D m_Camera;
			CGLSpriteRenderer m_SpriteRenderer;

			SGLTextureHandle m_TestTexture;

	};
}