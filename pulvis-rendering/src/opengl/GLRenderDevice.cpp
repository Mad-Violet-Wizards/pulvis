#include "GLRenderDevice.hpp"

#include <glad/glad.h>

namespace pulvis::rendering::gl
{
	SGLTextureHandle CGLRenderDevice::CreateTexture2D(const SGLTextureDesc& desc, const void* data)
	{
		SGLTextureHandle texture_handle{ 0 };

		glGenTextures(1, &texture_handle.ID);
		glBindTexture(GL_TEXTURE_2D, texture_handle.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = (desc.Channels == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, desc.Width, desc.Height, 0, format, GL_UNSIGNED_BYTE, data);

		if (desc.GenerateMipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		return texture_handle;
	}

	void CGLRenderDevice::DestroyTexture(SGLTextureHandle& _texture_handle)
	{
		if (_texture_handle.ID != 0)
		{
			GLuint id = _texture_handle.ID;
			glDeleteTextures(1, &id);
			_texture_handle.ID = 0;
		}
	}

	SGLBufferHandle CGLRenderDevice::CreateVertexBuffer(const void* _data, size_t _size, EBufferUsage _usage)
	{
		SGLBufferHandle buffer_handle;
		glGenBuffers(1, &buffer_handle.ID);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_handle.ID);
		const GLenum usage = (_usage == EBufferUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, _size, _data, usage);
		return buffer_handle;
	}

	SGLBufferHandle CGLRenderDevice::CreateIndexBuffer(const void* _data, size_t _size, EBufferUsage _usage)
	{
		SGLBufferHandle buffer_handle;
		glGenBuffers(1, &buffer_handle.ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_handle.ID);
		const GLenum usage = (_usage == EBufferUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, usage);
		return buffer_handle;
	}

	void CGLRenderDevice::DestroyBuffer(SGLBufferHandle& _buffer_handle)
	{
		if (_buffer_handle.ID != 0)
		{
			GLuint id = _buffer_handle.ID;
			glDeleteBuffers(1, &id);
			_buffer_handle.ID = 0;
		}
	}

	SGLVertexArrayHandle CGLRenderDevice::CreateVertexArray()
	{
		SGLVertexArrayHandle vertex_array_handle;
		glGenVertexArrays(1, &vertex_array_handle.ID);
		return vertex_array_handle;
	}

	void CGLRenderDevice::DestroyVertexArray(SGLVertexArrayHandle& _array_handle)
	{
		if (_array_handle.ID != 0)
		{
			glDeleteVertexArrays(1, &_array_handle.ID);
			_array_handle.ID = 0;
		}
	}

	void CGLRenderDevice::SetVertexLayout(SGLVertexArrayHandle _vao, SGLBufferHandle _vbo, const SGLVertexLayoutDesc& _layout)
	{
		glBindVertexArray(_vao.ID);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo.ID);

		for (const SGLVertexAttributeDesc& vertex_attribute : _layout.Attributes)
		{
			glEnableVertexAttribArray(vertex_attribute.Location);
			glVertexAttribPointer(
				vertex_attribute.Location,
				vertex_attribute.ComponentsCount,
				vertex_attribute.Type,
				vertex_attribute.Normalized,
				_layout.Stride,
				reinterpret_cast<const void*>(static_cast<uintptr_t>(vertex_attribute.Offset))
			);
		}

		glBindVertexArray(0);
	}

	void CGLRenderDevice::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}