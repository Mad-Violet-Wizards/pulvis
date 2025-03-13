#pragma once

#include "engine/filesystem/Filesystem.hpp"
#include "GameFileDataModel.hpp"

namespace engine::game
{
	class CGameContext
	{
		public:

			CGameContext(const std::string& _game_path);
			~CGameContext();

			engine::fs::Filesystem* GetFilesystem() const { return m_Filesystem; }

		private:

			engine::fs::Filesystem* m_Filesystem = nullptr;
	};
}
