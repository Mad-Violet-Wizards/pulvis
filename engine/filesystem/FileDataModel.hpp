#pragma once

namespace engine
{
namespace fs
{

	///////////////////////////////////////////////////////////////////////////////////////////////
	enum class EFileDataModelType
	{
		JSON,
		Binary,
		Text,
		Shader,
		Script,
		VertexShader,
		FragmentShader,
		GeometryShader,
		Png,
		Unknown
	};

	inline static std::string ToString(EFileDataModelType _file_data_model_type)
	{
		switch(_file_data_model_type)
		{
			case EFileDataModelType::JSON: return "JSON";
			case EFileDataModelType::Binary: return "Binary";
			case EFileDataModelType::Text: return "Text";
			case EFileDataModelType::Shader: return "Shader";
			case EFileDataModelType::Script: return "Script";
			case EFileDataModelType::Png: return "Png";
			case EFileDataModelType::Unknown: return "???";
			default: return "???";
		}
	}

	inline static EFileDataModelType FromFileExtension(const std::string& _file_extension)
	{
		if (_file_extension == ".json")
			return EFileDataModelType::JSON;
		else if (_file_extension == ".bin")
			return EFileDataModelType::Binary;
		else if (_file_extension == ".txt")
			return EFileDataModelType::Text;
		else if (_file_extension == ".lua")
			return EFileDataModelType::Script;
		else if (_file_extension == ".frag" || _file_extension == ".vert" || _file_extension == ".geom")
			return EFileDataModelType::Shader;
		else if (_file_extension == ".png")
			return EFileDataModelType::Png;
		else
			return EFileDataModelType::Unknown;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	class IFileDataModel
	{
		public:

			IFileDataModel() = default;
			virtual ~IFileDataModel() = default;

			IFileDataModel(const IFileDataModel&) = delete;
			IFileDataModel&operator=(const IFileDataModel&) = delete;

			virtual EFileDataModelType GetFileDataModelType() const = 0; 
	};
}
}
