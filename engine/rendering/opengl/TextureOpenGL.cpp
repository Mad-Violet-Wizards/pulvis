#include "engine/engine_pch.hpp"
#include "TextureOpenGL.hpp"
#include <vendor/common/include/glad/glad.h>

namespace engine::rendering::opengl
{
	CTextureOpenGL::CTextureOpenGL()
		: m_TextureID(-1)
		, m_Width(0)
		, m_Height(0)
		, m_Channels(0)
		, m_Data(nullptr)
	{
	}

	CTextureOpenGL::CTextureOpenGL(const std::string& _texture_name)
		: m_TextureName(_texture_name)
		, m_TextureID(-1)
		, m_Width(0)
		, m_Height(0)
		, m_Channels(0)
		, m_Data(nullptr)
	{

	}

	void CTextureOpenGL::LoadTexture(engine::fs::data_models::CPngFileDataModel* _png_texture)
	{
		unsigned int texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (_png_texture->GetChannels() == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _png_texture->GetWidth(), _png_texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _png_texture->GetBuffer());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (_png_texture->GetChannels() == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _png_texture->GetWidth(), _png_texture->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _png_texture->GetBuffer());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			PULVIS_WARNING_LOG("[CTextureOpenGL::LoadTexture] Unsupported number of channels: {}.", _png_texture->GetChannels());
		}
	}
	void CTextureOpenGL::BindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	void CTextureOpenGL::UnbindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
