#pragma once

#include <glm/glm.hpp>
#include "GLRenderDevice.hpp"
#include "GLShader.hpp"
#include "Camera2D.hpp"

namespace pulvis::rendering::gl
{
	class CGLSpriteRenderer
	{
		public:

			CGLSpriteRenderer(CGLRenderDevice& _render_device);
			~CGLSpriteRenderer();

			void Initialize();
			void Shutdown();

			void Draw(const CCamera2D& _camera, const glm::vec2& _pos, const glm::vec2& _size, SGLTextureHandle _texture);

		private:

			CGLShader m_Shader;
			CGLRenderDevice& m_Device;
			unsigned int m_Vao;
			SGLBufferHandle m_Vbo;
			SGLBufferHandle m_Ibo;

			bool m_Initialized;
	};
}