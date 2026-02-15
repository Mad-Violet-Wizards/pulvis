#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GLRenderDevice.hpp"
#include "GLShader.hpp"
#include "Camera2D.hpp"

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

			CGLTileRenderer(CGLRenderDevice& _render_device);
			~CGLTileRenderer();

			void Initialize();
			void Shutdown();

			void Draw(const std::vector<STileRenderBatch>& _batches, const CCamera2D& _camera);

		private:

			CGLRenderDevice& m_Device;
			CGLShader m_Shader;
			unsigned int m_Vao = 0;
			bool m_Initialized;
	};
}