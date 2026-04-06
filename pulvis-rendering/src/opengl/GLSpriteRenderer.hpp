#pragma once

#include <glm/glm.hpp>
#include "GLRenderDevice.hpp"
#include "GLShader.hpp"
#include "Camera2D.hpp"
#include "RenderQueue.hpp"

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering::gl
{
	class CGLSpriteRenderer
	{
		public:

			CGLSpriteRenderer(CGLRenderDevice& _render_device, pulvis::fs::assets::CAssetRegistry& _assets_registry);
			~CGLSpriteRenderer();

			void Initialize();
			bool IsInitialized() const { return m_Initialized; }
			void Shutdown();

			void Draw(const CCamera2D& _camera,
				const SSpriteDrawCmd& _cmd,
				const SFrameRenderState& _frame_state);

		private:

			bool LoadAndCompileShader(const std::string& _vert_path, const std::string& _frag_path);

		private:

			CGLShader m_Shader;
			CGLRenderDevice& m_Device;
			pulvis::fs::assets::CAssetRegistry& m_AssetsRegistry;

			unsigned int m_Vao;
			SGLBufferHandle m_Vbo;
			SGLBufferHandle m_Ibo;

			bool m_Initialized;
	};
}