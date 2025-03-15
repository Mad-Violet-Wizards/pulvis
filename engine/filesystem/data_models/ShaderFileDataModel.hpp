#pragma once

#include "engine/filesystem/FileDataModel.hpp"

namespace engine::rendering::opengl
{
	enum class EShaderType;
}

namespace engine::fs::data_models
{
	class CShaderFileDataModel : public IFileDataModel
	{
	public:
		CShaderFileDataModel(engine::rendering::opengl::EShaderType _shader_type);
		void Deserialize(std::fstream& _file_stream);

		EFileDataModelType GetFileDataModelType() const override { return EFileDataModelType::Shader; }
		engine::rendering::opengl::EShaderType GetShaderType() const { return m_ShaderType; }
		const char* GetShaderSource() const;

	private:

		std::string m_ShaderSource;
		engine::rendering::opengl::EShaderType m_ShaderType;
	};
}
