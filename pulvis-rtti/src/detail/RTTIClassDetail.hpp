#pragma once
#pragma warning(push)
#pragma warning(disable: 4251)

#include "RTTITypeTraits.hpp"
#include "RTTIClass.hpp"
#include "DynamicLibraryExport.hpp"

namespace pulvis::rtti::detail
{
	class PULVIS_DLL_API CRTTIClassStorage
	{
	public:

		static void RegisterClass(CRTTIClass* _class);

		[[nodiscard]] static CRTTIClass* FindClassById(type_id_t _id);
		[[nodiscard]] static const CRTTIClass* FindConstClassById(type_id_t _type_id);

	private:

		static std::unordered_map<type_id_t, CRTTIClass*> CLASSES;
	};

	static void AttachParent(const char* _class_name, const char* _parent_name);

	template<typename... Parents>
	static void RegisterParents(const char* _classname)
	{
		(AttachParent(_classname, std::string(pulvis::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
	}
}
#pragma warning(pop)