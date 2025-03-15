#pragma once

#include "engine/filesystem/data_models/ScriptFileDataModel.hpp"
#include "engine/vendor/sol3/sol.hpp"

namespace engine::scriptable
{
	class CScriptableService : public Singleton<CScriptableService>
	{
		public:

			CScriptableService();
			~CScriptableService();

			void Initialize();
			void SetupScripts(std::unordered_map<std::string, std::shared_ptr<fs::CScriptFileDataModel>>* _scripts);

			void InvokeScript(const std::string& _script_name);

		private:

			std::unordered_map<std::string, std::shared_ptr<fs::CScriptFileDataModel>>* m_Scripts;
			sol::state m_LuaState;
	};
}