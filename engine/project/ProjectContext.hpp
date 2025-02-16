#pragma once

#include "engine/filesystem/Filesystem.hpp"
#include "ProjectFileDataModel.hpp"

namespace engine::projects
{
	class CProjectContext
	{
		public:

			CProjectContext(CProjectFileDataModel* _file_data_model);
			~CProjectContext();

			engine::fs::Filesystem* GetFilesystem() const { return m_Filesystem; }

		private:

			CProjectFileDataModel* m_FileDataModel;
			engine::fs::Filesystem* m_Filesystem = nullptr;
	};
}
