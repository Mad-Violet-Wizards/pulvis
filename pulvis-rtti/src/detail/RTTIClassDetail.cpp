#include "RTTIClassDetail.hpp"
#include "Hash.hpp"

namespace pulvis::rtti::detail
{
	std::unordered_map<type_id_t, CRTTIClass*> CRTTIClassStorage::CLASSES;

	void CRTTIClassStorage::RegisterClass(CRTTIClass* _class)
	{
		CRTTIClassStorage::CLASSES.try_emplace(_class->GetTypeId(), _class);
	}

	CRTTIClass* CRTTIClassStorage::FindClassById(type_id_t _id)
	{
		auto it = CRTTIClassStorage::CLASSES.find(_id);
		return it != CRTTIClassStorage::CLASSES.end() ? it->second : nullptr;
	}

	const CRTTIClass* CRTTIClassStorage::FindConstClassById(type_id_t _type_id)
	{
		const auto it = CRTTIClassStorage::CLASSES.find(_type_id);
		return it != CRTTIClassStorage::CLASSES.end() ? it->second : nullptr;
	}

	void AttachParent(const char* _class_name, const char* _parent_name)
	{
		const type_id_t class_type_id = tl::hash::fnv1a<type_id_t>(_class_name);
		const type_id_t parent_type_id = tl::hash::fnv1a<type_id_t>(_parent_name);

		pulvis::rtti::detail::CRTTIClassStorage::FindClassById(class_type_id)->AttachParent(pulvis::rtti::detail::CRTTIClassStorage::FindClassById(parent_type_id));
	}

}