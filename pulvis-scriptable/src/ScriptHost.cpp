#include "ScriptHost.hpp"

#include "Logger.hpp"

#include <fstream>
#include <sstream>

namespace pulvis::scriptable
{
	CScriptHost::CScriptHost()
	{
		m_LuaState.open_libraries(
			sol::lib::base,
			sol::lib::string,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);
	}

	bool CScriptHost::RunString(std::string_view _code, std::string_view _chunk_name)
	{
		sol::protected_function_result result = m_LuaState.safe_script(
			_code,
			[](lua_State*, sol::protected_function_result _r) { return _r; },
			std::string{ _chunk_name }
		);

		if (!result.valid())
		{
			sol::error err = result;
			ReportError(_chunk_name, err.what());
			return false;
		}
		return true;
	}

	bool CScriptHost::RunFile(std::string_view _file_path)
	{
		std::ifstream file(std::string{ _file_path }, std::ios::in | std::ios::binary);
		if (!file)
		{
			ReportError(_file_path, "Cannot open file.");
			return false;
		}

		std::ostringstream ss;
		ss << file.rdbuf();
		return RunString(ss.str(), _file_path);
	}

	sol::environment CScriptHost::MakeEnvironment() const
	{
		// Inherits global table - script can call print(), math.* etc.
		// Locals defined inside the env stay inside the env.
		return sol::environment(m_LuaState, sol::create, m_LuaState.globals());
	}

	bool CScriptHost::RunStringInEnv(sol::environment& _env, std::string_view _code, std::string_view _chunk_name) const
	{
		sol::protected_function_result result = m_LuaState.safe_script(
			_code,
			_env,
			[](lua_State*, sol::protected_function_result _r) { return _r; },
			std::string{ _chunk_name }
		);

		if (!result.valid())
		{
			sol::error err = result;
			ReportError(_chunk_name, err.what());
			return false;
		}
		return true;
	}

	void CScriptHost::ReportError(std::string_view _context, std::string_view _message) const
	{
		PULVIS_ERROR_LOG("[Script:{}] {}", _context, _message);
	}
}