#include "engine/engine_pch.hpp"
#include "TextFileDataModel.hpp"

#include <fstream>


namespace engine::fs::data_models
{
	void CTextFileDataModel::Serialize(std::fstream& _file_stream)
	{
		for (const std::string& line : m_Lines)
		{
			_file_stream << line << std::endl;
		}
	}

	void CTextFileDataModel::Deserialize(std::fstream& _file_stream)
	{
		for (std::string line; std::getline(_file_stream, line); )
		{
			m_Lines.push_back(line);
		}
	}

	void CTextFileDataModel::SetLines(const std::vector<std::string>& _lines)
	{
		m_Lines = _lines;
	}

	const std::vector<std::string>& CTextFileDataModel::GetLines() const
	{
		return m_Lines;
	}

}