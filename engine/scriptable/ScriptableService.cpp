#include "engine/engine_pch.hpp"
#include "ScriptableService.hpp"

#include "engine/rtti/RTTIClass.hpp"

#include "engine/resources/TilesScriptable.hpp"

namespace engine::scriptable
{
	CScriptableService::CScriptableService()
	{
	}

	CScriptableService::~CScriptableService()
	{
	}

	void CScriptableService::Initialize()
	{
		m_LuaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::math, sol::lib::os);

		resources::CTile::ExportScriptable(m_LuaState);
		resources::CAtlasTile::ExportScriptable(m_LuaState);
	}

	void CScriptableService::SetupScripts(std::unordered_map<std::string, std::shared_ptr<fs::CScriptFileDataModel>>* _scripts)
	{
		m_Scripts = _scripts;
	}

}