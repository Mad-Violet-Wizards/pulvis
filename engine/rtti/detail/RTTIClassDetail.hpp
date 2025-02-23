#pragma once

#include "engine/rtti/RTTITypeTraits.hpp"
#include "engine/rtti/RTTIClass.hpp"

#include <vendor/common/include/sol3/sol.hpp>

namespace engine::rtti::detail
{
	class CRTTIClassStorage
	{
	public:

		static void RegisterClass(CRTTIClass* _class);

		static CRTTIClass* FindClassByName(const char* _name);
		static const CRTTIClass* FindConstClassByName(const char* _name);

		static CRTTIClass* FindClassById(type_id_t _id);
		static const CRTTIClass* FindConstClassById(type_id_t _type_id);

		static void ExportRTTI(sol::state& _lua_state);

	private:

		static inline int s_CurrentStorageIndex = 0;
		constexpr static inline int s_ClassBufferLimit = 2048;
		static inline CRTTIClass* s_ClassesStorage[s_ClassBufferLimit];

	};

	static void AttachParent(const char* _class_name, const char* _parent_name);

	template<typename... Parents>
	static void RegisterParents(const char* _classname)
	{
		(AttachParent(_classname, std::string(engine::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
	}
}