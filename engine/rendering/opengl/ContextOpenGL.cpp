#include "engine/engine_pch.hpp"
#include "ContextOpenGL.hpp"
#include "ShaderOpenGL.hpp"
#include "TextureOpenGL.hpp"

namespace engine::rendering::opengl
{
	CContext::CContext(engine::rendering::CWindow* _window)
	{
		InitializeRenderLayers();
	}

	CContext::~CContext()
	{
	}

	CShaderOpenGL* CContext::FindShader(const std::string& _shader_name) const
	{
		for (CShaderOpenGL* shader : m_Shaders)
		{
			if (shader->GetShaderName() == _shader_name)
			{
				return shader;
			}
		}

		return nullptr;
	}

	void CContext::SetupShaders(std::vector<CShaderOpenGL*>&& _shaders_vec)
	{
		m_Shaders = std::move(_shaders_vec);
	}

	CTextureOpenGL* CContext::FindTexture(const std::string& _texture_name) const
	{
		for (CTextureOpenGL* texture : m_Textures)
		{
			if (texture->GetTextureName() == _texture_name)
			{
				return texture;
			}
		}

		return nullptr;
	}

	void CContext::SetupTextures(std::vector<CTextureOpenGL*>&& _textures_vec)
	{
		m_Textures = std::move(_textures_vec);
	}

	void CContext::InitializeRenderLayers()
	{
		for (size_t i = 0; i < s_RenderLayersCount; ++i)
		{
			m_RenderLayers[i] = new CRenderLayerOpenGL(static_cast<ERenderLayerOpenGL>(i));
		}
	}
}