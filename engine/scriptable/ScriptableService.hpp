#pragma once

#include "engine/filesystem/ScriptFileDataModel.hpp"
#include <vendor/common/include/sol3/sol.hpp>

namespace engine::scriptable
{
	class CScriptableService : public Singleton<CScriptableService>
	{
		public:

			CScriptableService();
			~CScriptableService();

			void Initialize();
			void SetupScripts(std::unordered_map<std::string, std::shared_ptr<fs::CScriptFileDataModel>>* _scripts);

		private:

			std::unordered_map<std::string, std::shared_ptr<fs::CScriptFileDataModel>>* m_Scripts;
			sol::state m_LuaState;
	};
}