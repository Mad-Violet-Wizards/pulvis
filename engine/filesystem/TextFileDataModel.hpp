#pragma once

#include "FileDataModel.hpp"

namespace engine::fs
{
	class CTextFileDataModel : public IFileDataModel
	{
	public:

		CTextFileDataModel() = default;
		void Serialize(std::fstream& _file_stream);
		void Deserialize(std::fstream& _file_stream);

		void SetLines(const std::vector<std::string>& _lines);
		const std::vector<std::string>& GetLines() const;

		EFileDataModelType GetFileDataModelType() const override { return EFileDataModelType::Text; }

	private:

		std::vector<std::string> m_Lines;
	};

}