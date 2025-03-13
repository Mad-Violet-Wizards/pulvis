#include "engine/engine_pch.hpp"
#include "LuaScriptableAPI.hpp"

namespace engine::scriptable
{
	template<class C>
	inline void LuaScriptableAPI::ExportScriptableClassFields(sol::table& table, const rtti::CRTTIClass* rtti_class, C* _instance)
	{
	}
}
