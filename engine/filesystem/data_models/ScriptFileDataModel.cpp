#include "engine/engine_pch.hpp"
#include "ScriptFileDataModel.hpp"

#include <fstream>

namespace engine::fs
{
	void CScriptFileDataModel::Deserialize(std::fstream& _file_stream)
	{
		std::string l;
		while (std::getline(_file_stream, l))
		{
			m_Script += l + "\n";
		}
	}

	const std::string& CScriptFileDataModel::GetScript() const
	{
		return m_Script;
	}
}