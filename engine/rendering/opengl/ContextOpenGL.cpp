#include "engine/engine_pch.hpp"
#include "ContextOpenGL.hpp"
#include "Shader.hpp"

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

	CShader* CContext::FindShader(const std::string& _shader_name)
	{
		for (CShader* shader : m_Shaders)
		{
			if (shader->GetShaderName() == _shader_name)
			{
				return shader;
			}
		}

		return nullptr;
	}

	void CContext::SetupShaders(const std::vector<CShader*>& _shaders_vec)
	{
		m_Shaders = _shaders_vec;
	}
}