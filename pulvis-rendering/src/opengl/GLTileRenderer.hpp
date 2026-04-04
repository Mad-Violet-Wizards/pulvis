#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GLRenderDevice.hpp"
#include "GLShader.hpp"
#include "Camera2D.hpp"
#include "RenderQueue.hpp"

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering::gl
{
	struct STileVertex
	{
		glm::vec2 Position;
		glm::vec2 TextureCoordinates;
	};

	struct STileRenderBatch
	{
		SGLTextureHandle TextureHandle;
		SGLBufferHandle BufferHandle;
		glm::vec2 TextureSize;
		size_t VertexCount = 0;
	};

	class CGLTileRenderer
	{
		public:

			CGLTileRenderer(CGLRenderDevice& _render_device, pulvis::fs::assets::CAssetRegistry& _assets_registry);
			~CGLTileRenderer();

			void Initialize();
			bool IsInitialized() const { return m_Initialized; }
			void Shutdown();

			void Draw(const std::vector<STileDrawCmd>& _batches,
				const CCamera2D& _camera,
				const SFrameRenderState& _frame_state);

		private:

			bool LoadAndCompileShader(const std::string& _vert_path, const std::string& _frag_path);

		private:

			CGLRenderDevice& m_Device;
			pulvis::fs::assets::CAssetRegistry& m_AssetsRegistry;
			CGLShader m_Shader;
			unsigned int m_Vao = 0;
			bool m_Initialized;
	};
}