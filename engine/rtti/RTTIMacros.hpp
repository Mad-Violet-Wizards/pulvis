#pragma once

#include "RTTITypeTraits.hpp"
#include "detail/RTTIEnumDetail.hpp"

#define RTTI_CLASS_API(className, ...)																							 \
public:																																							 \
	virtual engine::rtti::type_id_t GetTypeId() const override												 \
	{																																									 \
		return engine::rtti::CRTTITypeInfo<className, ##__VA_ARGS__>::GetTypeId();			 \
	}																																									 \
	virtual bool IsTypeOfId(engine::rtti::type_id_t _type) const override							 \
	{																																									 \
		return engine::rtti::CRTTITypeInfo<className, ##__VA_ARGS__>::IsTypeOfId(_type); \
	}																																								   \
																																										 \

#define RTTI_ENUM_API(enumName) static engine::rtti::CRTTIEnum<enumName> s_Enum##enumName = {};

