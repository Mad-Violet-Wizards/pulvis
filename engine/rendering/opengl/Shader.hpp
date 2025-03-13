#pragma once

#include "engine/filesystem/ShaderFileDataModel.hpp"

namespace engine::rendering::opengl
{
	enum class EShaderType
	{
		Unknown,
		Vertex,
		Fragment,
		Geometry
	};

	inline static std::string ToString(EShaderType _shader_type)
	{
		switch (_shader_type)
		{
		case EShaderType::Vertex:
			return "Vertex";
		case EShaderType::Fragment:
			return "Fragment";
		case EShaderType::Geometry:
			return "Geometry";
		default:
			return "Unknown";
		}
	}

	inline static EShaderType FromFileExtension(const std::string& _file_extension)
	{
		if (_file_extension == ".vert")
		{
			return EShaderType::Vertex;
		}
		else if (_file_extension == ".frag")
		{
			return EShaderType::Fragment;
		}
		else if (_file_extension == ".geom")
		{
			return EShaderType::Geometry;
		}

		return EShaderType::Unknown;
	}
	

	class CShader
	{
		public:

			CShader(const std::string& _shader_name);
			void LoadShader(engine::fs::CShaderFileDataModel* _vertex_file_model, engine::fs::CShaderFileDataModel* _fragment_file_model);
			void LoadShader(engine::fs::CShaderFileDataModel* _vertex_file_model, engine::fs::CShaderFileDataModel* _fragment_file_model, engine::fs::CShaderFileDataModel* _geometry_file_model);
			~CShader() = default;

			void Bind();
			void Unbind();

			const std::string& GetShaderName() const { return m_ShaderName; }

	private:

		unsigned int CompileShader(const char* _shader_source, EShaderType _shader_type) const;

		unsigned int m_ID;
		std::string m_ShaderName;
	};
}