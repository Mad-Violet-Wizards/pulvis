#pragma once

#include "engine/filesystem/FileDataModel.hpp"

namespace engine::fs::data_models
{
	class CScriptFileDataModel : public engine::fs::IFileDataModel
	{
		public:

			CScriptFileDataModel() = default;
			~CScriptFileDataModel() = default;

			void Deserialize(std::fstream& _file_stream);
			EFileDataModelType GetFileDataModelType()	const override { return EFileDataModelType::Script; }

			const std::string& GetScript() const;

		private:

			std::string m_Script;
	};
}
