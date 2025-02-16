#include "engine/engine_pch.hpp"
#include "Shader.hpp"
#include <vendor/common/include/glad/glad.h>

namespace engine::rendering::opengl
{
	CShader::CShader(const std::string& _shader_name)
		: m_ShaderName(_shader_name)
	{
	}
	void CShader::LoadShader(engine::fs::CShaderFileDataModel* _vertex_file_model, engine::fs::CShaderFileDataModel* _fragment_file_model)
	{
		const char* vertex_shader_source = _vertex_file_model->GetShaderSource();
		const char* fragment_shader_source = _fragment_file_model->GetShaderSource();

		const unsigned int vertex_shader_id = CompileShader(vertex_shader_source, EShaderType::Vertex);
		const unsigned int fragment_shader_id = CompileShader(fragment_shader_source, EShaderType::Fragment);

		const int program = glCreateProgram();
		glAttachShader(program, vertex_shader_id);
		glAttachShader(program, fragment_shader_id);
		glLinkProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char info_log[512];
			glGetProgramInfoLog(program, 512, NULL, info_log);
			std::cout << "[CShader::LoadShader] PROGRAM_LINKING_FAILED\n" << info_log << std::endl;
		}
		else
		{
			m_ID = program;
		}

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}
	void CShader::LoadShader(engine::fs::CShaderFileDataModel* _vertex_file_model, engine::fs::CShaderFileDataModel* _fragment_file_model, engine::fs::CShaderFileDataModel* _geometry_file_model)
	{
	}

	void CShader::Bind()
	{
		glUseProgram(m_ID);
	}

	void CShader::Unbind()
	{
		glUseProgram(0);
	}

	unsigned int CShader::CompileShader(const char* _shader_source, EShaderType _shader_type) const
	{
		const auto gl_shader_type = [_shader_type]()
		{
			switch (_shader_type)
			{
			case EShaderType::Vertex:
				return GL_VERTEX_SHADER;
			case EShaderType::Fragment:
				return GL_FRAGMENT_SHADER;
			case EShaderType::Geometry:
				return GL_GEOMETRY_SHADER;
			default:
				return 0;
			}
		}();

		unsigned int shader_id = glCreateShader(gl_shader_type);
		glShaderSource(shader_id, 1, &_shader_source, NULL);
		glCompileShader(shader_id);
		
		int success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char info_log[512];
			glGetShaderInfoLog(shader_id, 512, NULL, info_log);
			std::cout << "[CShader::CompileShader]" << ToString(_shader_type) << " COMPILATION_FAILED\n" << info_log << std::endl;
		}

		return shader_id;
	}
}
