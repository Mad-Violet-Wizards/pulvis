#include "engine/engine_pch.hpp"
#include "ScriptableService.hpp"

#include "engine/rtti/RTTIClass.hpp"

#include "engine/resources/ScriptTile.hpp"

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

		resources::CScriptTile::ExportScriptable(m_LuaState);
		resources::CScriptAtlasTile::ExportScriptable(m_LuaState);
	}

	void CScriptableService::SetupScripts(std::unordered_map<std::string, std::shared_ptr<engine::fs::data_models::CScriptFileDataModel>>&& _scripts)
	{
		m_Scripts = std::move(_scripts);
	}

	void CScriptableService::InvokeScript(const std::string& _script_name)
	{
		auto script = m_Scripts.find(_script_name);
		if (script != m_Scripts.end())
		{
			const sol::load_result result = m_LuaState.load(script->second->GetScript());
			if (result.valid())
			{
				const sol::protected_function script_function = result;
				const sol::protected_function_result script_result = script_function();
				if (!script_result.valid())
				{
					const sol::error err = script_result;
					PULVIS_ERROR_LOG("Error invoking script: {0}", err.what());
				}
			}
			else
			{
				const sol::error err = result;
				PULVIS_ERROR_LOG("Error loading script: {0}", err.what());
			}
		}
		else
		{
			PULVIS_ERROR_LOG("Script not found: {0}", _script_name);
		}
	}

}