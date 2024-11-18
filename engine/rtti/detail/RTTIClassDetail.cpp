#include "engine/engine_pch.hpp"
#include "RTTIClassDetail.hpp"


namespace engine::rtti::detail
{
	//CRTTIClass* class_detail::FindClassById(type_id_t _id)
	//{
	//	for (CRTTIClass& c : s_ClassesStorage)
	//	{
	//		if (c.GetTypeId() == _id)
	//		{
	//			return &c;
	//		}
	//	}
	//	return nullptr;
	//}
	//const CRTTIClass* class_detail::FindConstClassById(type_id_t _id)
	//{
	//	for (const CRTTIClass& c : s_ClassesStorage)
	//	{
	//		if (c.GetTypeId() == _id)
	//		{
	//			return &c;
	//		}
	//	}
	//	return nullptr;
	//}

	//const CRTTIClass* class_detail::FindConstClassByName(const char* _name)
	//{
	//	for (const CRTTIClass& c : s_ClassesStorage)
	//	{
	//		if (c.GetName() == _name)
	//		{
	//			return &c;
	//		}
	//	}
	//	return nullptr;
	//}

	void CRTTIClassStorage::RegisterClass(CRTTIClass* _class)
	{
		s_ClassesStorage[s_CurrentStorageIndex++] = _class;
		static_assert(s_ClassesStorage.size() == s_ClassBufferLimit, "RTTI class buffer limit reached");
	}

	CRTTIClass* CRTTIClassStorage::FindClassByName(const char* _name)
	{
		for (CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetName() == _name)
			{
				return c;
			}
		}
		return nullptr;
	}

	const CRTTIClass* CRTTIClassStorage::FindConstClassByName(const char* _name)
	{
		for (const CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetName() == _name)
			{
				return c;
			}
		}
		return nullptr;
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
		engine::rtti::detail::CRTTIClassStorage::FindClassByName(_class_name)->AttachParent(engine::rtti::detail::CRTTIClassStorage::FindClassByName(_parent_name));
	}

}