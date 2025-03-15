#pragma once

#include "engine/filesystem/FileDataModel.hpp"

namespace engine::fs
{
	class CScriptFileDataModel : public IFileDataModel
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
