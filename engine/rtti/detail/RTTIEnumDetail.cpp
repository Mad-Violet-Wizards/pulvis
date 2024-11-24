#include "engine/engine_pch.hpp"
#include "RTTIEnumDetail.hpp"

namespace engine::rtti::detail
{
	void CRTTIEnumStorage::RegisterEnum(const SEnumDataBuffer& _enum_data_buffer)
	{
		s_EnumDataStorage[s_EnumMapperIndex] = _enum_data_buffer;
	}

	SEnumDataBuffer& CRTTIEnumStorage::GetEnumDataBufferRef(int _index)
	{
		return s_EnumDataStorage[_index];
	}

	const SEnumDataBuffer& CRTTIEnumStorage::GetEnumDataBufferConstRef(int _index)
	{
		return s_EnumDataStorage[_index];
	}

	int CRTTIEnumStorage::GetCurrentIndex()
	{
		return s_EnumMapperIndex;
	}

	void CRTTIEnumStorage::IncrementIndex()
	{
		s_EnumMapperIndex++;
	}
}