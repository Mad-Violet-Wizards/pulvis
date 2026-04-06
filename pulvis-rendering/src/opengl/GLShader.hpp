#pragma once

#include <string>
#include <glm/glm.hpp>

namespace pulvis::rendering::gl
{
	enum class EShaderType
	{
		Vertex,
		Fragment,
		Geometry,
		Unknown
	};

	std::string ToString (EShaderType _shader_type);
	EShaderType FromString(const std::string& _shader_type);

/////////////////////////////////////////////////////////////////////

	class CGLShader
	{
		public:

			CGLShader();
			CGLShader(const char* _vertex_shader_path, const char* _fragment_shader_path);
			CGLShader(const char* _vertex_shader_path, const char* _geometry_shader_path, const char* _fragment_shader_path);
			~CGLShader();

			CGLShader(const CGLShader&) = delete;
			CGLShader& operator=(const CGLShader&) = delete;

			CGLShader(CGLShader&& _other) noexcept;
			CGLShader& operator=(CGLShader&& _other) noexcept;

			bool CompileFromSource(const char* _vertex_shader_source, const char* _fragment_shader_source);
			bool CompileFromSource(const char* _vertex_shader_source, const char* _geometry_shader_source, const char* _fragment_shader_source);

			void Use() const;
			static void Unuse();

			void SetBool(const std::string& _name, bool _value) const;
			void SetInt(const std::string& _name, int _value) const;
			void SetFloat(const std::string& _name, float _value) const;

			void SetVec2(const std::string& _name, glm::vec2 _value) const;
			void SetVec3(const std::string& _name, glm::vec3 _value) const;
			void SetVec4(const std::string& _name, glm::vec4 _value) const;

			void SetMat3(const std::string& _name, const glm::mat3& _value) const;
			void SetMat4(const std::string& _name, const glm::mat4& _value) const;

			bool IsValid() const { return m_ID != 0; }
			unsigned int GetID() const;

		private:

			std::string LoadShaderFromFile(const std::string& _path) const;

			static bool CompileShader(unsigned int _shader_id, EShaderType _shader_type);
			static bool LinkProgram(unsigned int _program_id);

			void BuildProgram(const char* _vertex_shader_source, const char* _geometry_shader_source, const char* _fragment_shader_source);

			int GetUniformLocation(const char* _name) const;
			void Destroy();

		private:

			unsigned int m_ID;

	};
}
