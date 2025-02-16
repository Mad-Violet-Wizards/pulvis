#include "engine/engine_pch.hpp"
#include "ProjectContext.hpp"

namespace engine::projects
{
	CProjectContext::CProjectContext(CProjectFileDataModel* _file_data_model)
		: m_FileDataModel(_file_data_model)
	{
		m_Filesystem = new engine::fs::Filesystem("game", _file_data_model->GetPath());
	}

	CProjectContext::~CProjectContext()
	{
	}

}