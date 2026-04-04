#include "GLShader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Logger.hpp"
#include <fstream>

namespace pulvis::rendering::gl
{
	std::string ToString(EShaderType _shader_type)
	{
		switch (_shader_type)
		{
			case EShaderType::Fragment: return "Fragment";
			case EShaderType::Vertex: return "Vertex";
			case EShaderType::Geometry: return "Geometry";
			default: return "Unknown";
		}
	}

	EShaderType FromString(const std::string& _shader_type)
	{
		if (_shader_type == "Fragment")
		{
			return EShaderType::Fragment;
		}
		else if (_shader_type == "Vertex")
		{
			return EShaderType::Vertex;
		}
		else if (_shader_type == "Geometry")
		{
			return EShaderType::Geometry;
		}
		else
		{
			return EShaderType::Unknown;
		}
	}

/////////////////////////////////////////////////////////////////////

	CGLShader::CGLShader()
		: m_ID(0)
	{
	}

	CGLShader::CGLShader(const char* _vertex_shader_path, const char* _fragment_shader_path)
	{
		const std::string vertex_shader_source = LoadShaderFromFile(_vertex_shader_path);
		const std::string fragment_shader_source = LoadShaderFromFile(_fragment_shader_path);

		if (vertex_shader_source.empty() || fragment_shader_source.empty())
		{
			PULVIS_ERROR_LOG("Shader code not loaded correctly. Paths: {}\n {}", _vertex_shader_path, _fragment_shader_path);
			m_ID = 0;
			return;
		}

		BuildProgram(vertex_shader_source.c_str(), nullptr, fragment_shader_source.c_str());
	}

	CGLShader::CGLShader(const char* _vertex_shader_path, const char* _geometry_shader_path, const char* _fragment_shader_path)
	{
		const std::string vertex_shader_source = LoadShaderFromFile(_vertex_shader_path);
		const std::string geometry_shader_source = LoadShaderFromFile(_geometry_shader_path);
		const std::string fragment_shader_source = LoadShaderFromFile(_fragment_shader_path);

		if (vertex_shader_source.empty() || geometry_shader_source.empty() || fragment_shader_source.empty())
		{
			PULVIS_ERROR_LOG("Shader code not loaded correctly. Paths: {}\n {}\n {}", _vertex_shader_path, _geometry_shader_path, _fragment_shader_path);
			m_ID = 0;
			return;
		}

		BuildProgram(vertex_shader_source.c_str(), geometry_shader_source.c_str(), fragment_shader_source.c_str());
	}

	CGLShader::~CGLShader()
	{
		Destroy();
	}

	CGLShader::CGLShader(CGLShader&& _other) noexcept
	{
		m_ID = _other.m_ID;
		_other.m_ID = 0;
	}

	CGLShader& CGLShader::operator=(CGLShader&& _other) noexcept
	{
		if (this != &_other)
		{
			Destroy();
			m_ID = _other.m_ID;
			_other.m_ID = 0;
		}

		return *this;
	}

	bool CGLShader::CompileFromSource(const char* _vertex_shader_source, const char* _fragment_shader_source)
	{
		Destroy();

		if (_vertex_shader_source == nullptr || _fragment_shader_source == nullptr)
		{
			PULVIS_ERROR_LOG("Shader source code is null.");
			return false;
		}

		BuildProgram(_vertex_shader_source, nullptr, _fragment_shader_source);
		return IsValid();
	}

	bool CGLShader::CompileFromSource(const char* _vertex_shader_source, const char* _geometry_shader_source, const char* _fragment_shader_source)
	{
		Destroy();

		if (_vertex_shader_source == nullptr || _geometry_shader_source == nullptr || _fragment_shader_source == nullptr)
		{
			PULVIS_ERROR_LOG("Shader source code is null.");
			return false;
		}

		BuildProgram(_vertex_shader_source, _geometry_shader_source, _fragment_shader_source);
		return IsValid();
	}

	void CGLShader::Use() const
	{
		glUseProgram(m_ID);
	}

	void CGLShader::Unuse()
	{
		glUseProgram(0);
	}

	void CGLShader::SetBool(const std::string& _name, bool _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform1i(uniform_location, static_cast<int>(_value));
		}
	}

	void CGLShader::SetInt(const std::string& _name, int _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform1i(uniform_location, _value);
		}
	}

	void CGLShader::SetFloat(const std::string& _name, float _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform1f(uniform_location, _value);
		}
	}

	void CGLShader::SetVec2(const std::string& _name, glm::vec2 _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform2f(uniform_location, _value.x, _value.y);
		}
	}

	void CGLShader::SetVec3(const std::string& _name, glm::vec3 _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform3f(uniform_location, _value.x, _value.y, _value.z);
		}
	}

	void CGLShader::SetVec4(const std::string& _name, glm::vec4 _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniform4f(uniform_location, _value.x, _value.y, _value.z, _value.w);
		}
	}

	void CGLShader::SetMat3(const std::string& _name, const glm::mat3& _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(_value));
		}
	}

	void CGLShader::SetMat4(const std::string& _name, const glm::mat4& _value) const
	{
		const int uniform_location = GetUniformLocation(_name.c_str());

		if (uniform_location >= 0)
		{
			glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(_value));
		}
	}

	unsigned int CGLShader::GetID() const
	{
		return m_ID;
	}

	std::string CGLShader::LoadShaderFromFile(const std::string& _path) const
	{
		std::ifstream in_file_stream(_path, std::ios::binary);

		if (!in_file_stream.is_open())
		{
			PULVIS_ERROR_LOG("Failed to open shader file: {}", _path);
			return {};
		}

		in_file_stream.seekg(0, std::ios::end);
		std::streamsize size = in_file_stream.tellg();
		in_file_stream.seekg(0, std::ios::beg);

		std::string data(static_cast<size_t>(size), '\0');
		if (!in_file_stream.read(data.data(), size))
		{
			PULVIS_ERROR_LOG("Failed to read shader file: {}", _path);
			return {};
		}

		return data;
	}

	bool CGLShader::CompileShader(unsigned int _shader_id, EShaderType _shader_type)
	{
		int success = 0;
		char info_log[1024];

		glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(_shader_id, 1024, nullptr, info_log);
			PULVIS_ERROR_LOG("Shader compilation failed: {}\n {}", ToString(_shader_type), info_log);
			return false;
		}

		return true;
	}

	bool CGLShader::LinkProgram(unsigned int _program_id)
	{
		int success = 0;
		char info_log[1024];

		glGetProgramiv(_program_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(_program_id, 1024, nullptr, info_log);
			PULVIS_ERROR_LOG("Shader program link failed.\n{}", info_log);
			return false;
		}

		return true;
	}

	void CGLShader::BuildProgram(const char* _vertex_shader_source, const char* _geometry_shader_source, const char* _fragment_shader_source)
	{
		unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader_id, 1, &_vertex_shader_source, NULL);
		glCompileShader(vertex_shader_id);
		if (!CompileShader(vertex_shader_id, EShaderType::Vertex))
		{
			glDeleteShader(vertex_shader_id);
			m_ID = 0;
			return;
		}

		unsigned int geometry_shader_id = 0;
		if (_geometry_shader_source)
		{
			geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry_shader_id, 1, &_geometry_shader_source, NULL);
			glCompileShader(geometry_shader_id);
			if (!CompileShader(geometry_shader_id, EShaderType::Geometry))
			{
				glDeleteShader(vertex_shader_id);
				glDeleteShader(geometry_shader_id);
				m_ID = 0;
				return;
			}
		}

		unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader_id, 1, &_fragment_shader_source, NULL);
		glCompileShader(fragment_shader_id);
		if (!CompileShader(fragment_shader_id, EShaderType::Fragment))
		{
			glDeleteShader(vertex_shader_id);
			if (_geometry_shader_source)
			{
				glDeleteShader(geometry_shader_id);
			}

			glDeleteShader(fragment_shader_id);
			m_ID = 0;
			return;
		}

		unsigned int program_id = glCreateProgram();
		glAttachShader(program_id, vertex_shader_id);
		if (_geometry_shader_source)
		{
			glAttachShader(program_id, geometry_shader_id);
		}
		glAttachShader(program_id, fragment_shader_id);
		glLinkProgram(program_id);

		glDetachShader(program_id, vertex_shader_id);
		if (_geometry_shader_source)
		{
			glDetachShader(program_id, geometry_shader_id);
		}
		glDetachShader(program_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		if (_geometry_shader_source)
		{
			glDeleteShader(geometry_shader_id);
		}
		glDeleteShader(fragment_shader_id);

		if (!LinkProgram(program_id))
		{
			glDeleteProgram(program_id);
			m_ID = 0;
			return;
		}

		m_ID = program_id;
	}

	int CGLShader::GetUniformLocation(const char* _name) const
	{
		if (m_ID == 0)
		{
			return -1;
		}

		const int uniform_location = glGetUniformLocation(m_ID, _name);
		if (uniform_location == -1)
		{
			PULVIS_WARNING_LOG("Unform {} not found in shader.", _name);
		}

		return uniform_location;
	}

	void CGLShader::Destroy()
	{
		if (m_ID != 0)
		{
			glDeleteProgram(m_ID);
			m_ID = 0;
		}
	}
}
