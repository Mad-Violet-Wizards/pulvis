#include "RTTIEnumDetail.hpp"

namespace pulvis::rtti::detail
{
	std::unordered_map<pulvis::rtti::type_id_t, SEnumDataBuffer> CRTTIEnumStorage::ENUMS;

	void CRTTIEnumStorage::RegisterEnum(pulvis::rtti::type_id_t _id, SEnumDataBuffer _enum_data_buffer)
	{
		CRTTIEnumStorage::ENUMS.try_emplace(_id, _enum_data_buffer);
	}

	SEnumDataBuffer* CRTTIEnumStorage::GetEnumData(pulvis::rtti::type_id_t _index)
	{
		auto it = CRTTIEnumStorage::ENUMS.find(_index);
		return it != CRTTIEnumStorage::ENUMS.end() ? &it->second : nullptr;
	}
}