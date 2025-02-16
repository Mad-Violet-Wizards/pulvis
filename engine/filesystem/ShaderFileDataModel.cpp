#include "engine/engine_pch.hpp"
#include "engine/rendering/opengl/Shader.hpp"
#include "ShaderFileDataModel.hpp"
#include <fstream>

namespace engine::fs
{
	CShaderFileDataModel::CShaderFileDataModel(engine::rendering::opengl::EShaderType _shader_type)
		: m_ShaderType(_shader_type)
	{

	}

	void CShaderFileDataModel::Deserialize(std::fstream& _file_stream)
	{
		std::string l;
		while (std::getline(_file_stream, l))
		{
						m_ShaderSource += l + "\n";
		}
	}

	const char * CShaderFileDataModel::GetShaderSource() const
	{
		return m_ShaderSource.c_str();
	}
}