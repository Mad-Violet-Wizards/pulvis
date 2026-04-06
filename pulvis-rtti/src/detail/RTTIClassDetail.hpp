#pragma once

#include "RTTITypeTraits.hpp"
#include "RTTIClass.hpp"

namespace pulvis::rtti::detail
{
	class CRTTIClassStorage
	{
	public:

		static void RegisterClass(CRTTIClass* _class);

		static CRTTIClass* FindClassById(type_id_t _id);
		static const CRTTIClass* FindConstClassById(type_id_t _type_id);

	private:

		static inline int s_CurrentStorageIndex = 0;
		constexpr static inline int s_ClassBufferLimit = 2048;
		static inline CRTTIClass* s_ClassesStorage[s_ClassBufferLimit];

	};

	static void AttachParent(const char* _class_name, const char* _parent_name);

	template<typename... Parents>
	static void RegisterParents(const char* _classname)
	{
		(AttachParent(_classname, std::string(pulvis::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
	}
}