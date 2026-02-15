#pragma once

#include <cstdint>
#include <vector>
#include <glad/glad.h>

namespace pulvis::rendering::gl
{

//////////////////////////////////////////////////////////////////////////
	struct SGLTextureHandle { uint32_t ID = 0; };
	struct SGLBufferHandle { uint32_t ID = 0; };
	struct SGLVertexArrayHandle { uint32_t ID = 0; };
	struct SGLProgramHandle { uint32_t ID = 0; };
//////////////////////////////////////////////////////////////////////////
	enum class EBufferUsage
	{
		Static,
		Dynamic
	};

	struct SGLTextureDesc
	{
		int Width;
		int Height;
		int Channels;
		bool GenerateMipmaps;
	};
	
	struct SGLVertexAttributeDesc
	{
		uint32_t Location;
		uint32_t ComponentsCount;
		GLenum Type;
		GLboolean Normalized;
		uint32_t Offset;
	};

	struct SGLVertexLayoutDesc
	{
		uint32_t Stride;
		std::vector<SGLVertexAttributeDesc> Attributes;
	};
	//////////////////////////////////////////////////////////////////////////

	class CGLRenderDevice
	{
	public:

		SGLTextureHandle CreateTexture2D(const SGLTextureDesc& desc, const void* data);
		void DestroyTexture(SGLTextureHandle& _texture_handle);

		SGLBufferHandle CreateVertexBuffer(const void* _data, size_t _size, EBufferUsage _usage);
		SGLBufferHandle CreateIndexBuffer(const void* _data, size_t _size, EBufferUsage _usage);
		void DestroyBuffer(SGLBufferHandle& _buffer_handle);

		SGLVertexArrayHandle CreateVertexArray();
		void DestroyVertexArray(SGLVertexArrayHandle& _array_handle);

		void SetVertexLayout(SGLVertexArrayHandle _vao, 
											   SGLBufferHandle _vbo,
												 const SGLVertexLayoutDesc& _layout);

		void Clear(float r, float g, float b, float a);
	};

}