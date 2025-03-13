#pragma once
#include "engine/vendor/sol3/sol.hpp"
#include "engine/rtti/RTTIClass.hpp"

namespace engine::scriptable
{
	class LuaScriptableAPI
	{
		public:

			template<class C>
			static void ExportScriptableClassFields(sol::table& table, const rtti::CRTTIClass* rtti_class, C* _instance);
	};
}
