#include "RTTIClassDetail.hpp"


namespace pulvis::rtti::detail
{
	void CRTTIClassStorage::RegisterClass(CRTTIClass* _class)
	{
		s_ClassesStorage[s_CurrentStorageIndex++] = _class;
	}

	CRTTIClass* CRTTIClassStorage::FindClassById(type_id_t _id)
	{
		for (CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetTypeId() == _id)
			{
				return c;
			}
		}
		return nullptr;
	}

	const CRTTIClass* CRTTIClassStorage::FindConstClassById(type_id_t _type_id)
	{
		for (const CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetTypeId() == _type_id)
			{
				return c;
			}
		}
		return nullptr;
	}

	void AttachParent(const char* _class_name, const char* _parent_name)
	{
		const type_id_t class_type_id = rtti::Hash(_class_name);
		const type_id_t parent_type_id = rtti::Hash(_parent_name);

		pulvis::rtti::detail::CRTTIClassStorage::FindClassById(class_type_id)->AttachParent(pulvis::rtti::detail::CRTTIClassStorage::FindClassById(parent_type_id));
	}

}