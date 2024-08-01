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
		Text, // Unimplemented yet
		Unknown
	};

	inline static std::string ToString(EFileDataModelType _file_data_model_type)
	{
		switch(_file_data_model_type)
		{
			case EFileDataModelType::JSON: return "JSON";
			case EFileDataModelType::Binary: return "Binary";
			case EFileDataModelType::Text: return "Text";
			case EFileDataModelType::Unknown: return "???";
			default: return "???";
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	class PULVIS_API IFileDataModel
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
