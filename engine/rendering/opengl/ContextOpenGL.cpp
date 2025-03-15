#include "engine/engine_pch.hpp"
#include "ContextOpenGL.hpp"
#include "ShaderOpenGL.hpp"

namespace engine::rendering::opengl
{
	CContext::CContext()
	{

	}

	CContext::CContext(engine::rendering::CWindow* _window)
	{
	}

	CContext::~CContext()
	{
	}

	CShaderOpenGL* CContext::FindShader(const std::string& _shader_name)
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

	void CContext::SetupShaders(const std::vector<CShaderOpenGL*>& _shaders_vec)
	{
		m_Shaders = _shaders_vec;
	}
}