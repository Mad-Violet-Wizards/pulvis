#include "engine/engine_pch.hpp"
#include "RTTIClassDetail.hpp"


namespace engine::rtti::detail
{
	void CRTTIClassStorage::RegisterClass(CRTTIClass* _class)
	{
		s_ClassesStorage[s_CurrentStorageIndex++] = _class;
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

	CRTTIClass* CRTTIClassStorage::FindClassByNameSv(std::string_view _sv_name)
	{
		for (CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetName() == _sv_name)
			{
				return c;
			}
		}
		return nullptr;
	}

	const CRTTIClass* CRTTIClassStorage::FindConstClassByNameSv(std::string_view _sv_name)
	{
		for (const CRTTIClass* c : s_ClassesStorage)
		{
			if (c && c->GetName() == _sv_name)
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