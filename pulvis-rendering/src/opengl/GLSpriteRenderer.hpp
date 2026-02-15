#pragma once

#include <glm/glm.hpp>
#include "GLRenderDevice.hpp"
#include "GLShader.hpp"

namespace pulvis::rendering::gl
{
	class CGLSpriteRenderer
	{
		public:

			CGLSpriteRenderer(CGLRenderDevice& _render_device);
			~CGLSpriteRenderer();

			void Initialize();
			void Shutdown();

			void Begin(const glm::mat4& _viewProj);
			void Draw(const glm::vec2& _pos, const glm::vec2& _size, SGLTextureHandle _texture);
			void End();

		private:

			CGLShader m_Shader;
			CGLRenderDevice& m_Device;
			unsigned int m_Vao;
			SGLBufferHandle m_Vbo;
			SGLBufferHandle m_Ibo;

			glm::mat4 m_ViewProj;
			bool m_Initialized;
	};
}