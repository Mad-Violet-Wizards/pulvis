#include "engine/engine_pch.hpp"
#include "TextureOpenGL.hpp"

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

	void CTextureOpenGL::LoadTexture(const std::string& _texture_path)
	{

	}
}
