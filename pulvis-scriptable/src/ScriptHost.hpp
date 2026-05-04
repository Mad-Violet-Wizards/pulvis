#pragma once

#include <sol3/sol.hpp>

#include <string>
#include <string_view>

namespace pulvis::scriptable
{
	/*
		Thin RAII wrapper around sol::state.

		The script host owns ONE sol::state. Higher-level systems
		(level layers, worldgen, AI, dialogue) borrow it via
		CScriptableService::GetHost() and run their scripts inside
		isolated sol::environment instances - so per-script globals
		(like BuildLayers, GeneratePlanet) never collide.

		Use RunStringInEnv() / CallInEnv() for sandboxed execution.
		Use RunString() / Call() only for genuinely global helpers.
	*/
	class CScriptHost
	{
	public:

		CScriptHost();
		~CScriptHost() = default;

		CScriptHost(const CScriptHost&) = delete;
		CScriptHost& operator=(const CScriptHost&) = delete;

		CScriptHost(CScriptHost&&) = default;
		CScriptHost& operator=(CScriptHost&&) = default;

		// Global execution (BuildLayers/GeneratePlanet/... become globals).
		bool RunString(std::string_view _code, std::string_view _chunk_name = "chunk");
		bool RunFile(std::string_view _file_path);

		// Sandboxed execution. Returns a fresh environment that inherits
		// the standard library + global helpers; callers run scripts in it
		// and pull functions out without touching the global namespace.
		sol::environment MakeEnvironment() const;

		bool RunStringInEnv(sol::environment& _env, std::string_view _code, std::string_view _chunk_name = "chunk") const;

		template<typename Result = sol::object, typename... Args>
		sol::optional<Result> CallInEnv(const sol::environment& _env, std::string_view _function_name, Args&&... _args) const
		{
			sol::protected_function fn = _env[_function_name];
			return InvokeFunction<Result>(fn, _function_name, std::forward<Args>(_args)...);
		}

		template<typename Result = sol::object, typename... Args>
		sol::optional<Result> Call(std::string_view _function_name, Args&&... _args) const
		{
			sol::protected_function fn = m_LuaState[_function_name];
			return InvokeFunction<Result>(fn, _function_name, std::forward<Args>(_args)...);
		}

		sol::state& Raw() { return m_LuaState; }
		const sol::state& Raw() const { return m_LuaState; }

	private:

		template<typename Result, typename... Args>
		sol::optional<Result> InvokeFunction(sol::protected_function& _fn, std::string_view _name, Args&&... _args) const
		{
			if (!_fn.valid())
			{
				return sol::nullopt;
			}

			sol::protected_function_result result = _fn(std::forward<Args>(_args)...);
			if (!result.valid())
			{
				sol::error err = result;
				ReportError(_name, err.what());
				return sol::nullopt;
			}

			if constexpr (std::is_same_v<Result, void>)
			{
				return sol::optional<Result>{};
			}
			else
			{
				return result.get<Result>();
			}
		}

		void ReportError(std::string_view _context, std::string_view _message) const;

	private:

		mutable sol::state m_LuaState;
	};
}