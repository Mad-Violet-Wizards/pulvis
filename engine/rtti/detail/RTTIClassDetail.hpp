#pragma once

#include "engine/rtti/RTTITypeTraits.hpp"
#include "engine/rtti/RTTIClass.hpp"
#include "engine/core/Export.hpp"

namespace engine::rtti::detail
{
	class PULVIS_API CRTTIClassStorage
	{
	public:

		static void RegisterClass(CRTTIClass* _class);

		static CRTTIClass* FindClassByName(const char* _name);
		static const CRTTIClass* FindConstClassByName(const char* _name);

		static CRTTIClass* FindClassById(type_id_t _id);
		static const CRTTIClass* FindConstClassById(type_id_t _type_id);

	private:

		static inline int s_CurrentStorageIndex = 0;
		constexpr static inline int s_ClassBufferLimit = 2048;
		static inline std::array<CRTTIClass*, s_ClassBufferLimit> s_ClassesStorage = {};
	};

	static void AttachParent(const char* _class_name, const char* _parent_name);

	template<typename... Parents>
	static void RegisterParents(const char* _classname)
	{
		(AttachParent(_classname, std::string(engine::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
	}
}