#pragma once

#include "RTTITypeTraits.hpp"
#include "RTTITypeName.hpp"
#include "RTTITypeInfo.hpp"
#include "RTTIMethodInfo.hpp"
#include "RTTIMethod.hpp"
#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "Hash.hpp"
#include "detail/RTTIClassDetail.hpp"

//////////////////////////////////////////////////////////////////////////
static void AttachParent(const char* _class_name, const char* _parent_name)
{
	const pulvis::rtti::type_id_t class_type_id = pulvis::tl::hash::fnv1a<pulvis::rtti::type_id_t>(_class_name);
	const pulvis::rtti::type_id_t parent_type_id = pulvis::tl::hash::fnv1a<pulvis::rtti::type_id_t>(_parent_name);

	pulvis::rtti::detail::CRTTIClassStorage::FindClassById(class_type_id)->AttachParent(pulvis::rtti::detail::CRTTIClassStorage::FindClassById(parent_type_id));
}

template<typename... Parents>
static void RegisterParents(const char* _classname)
{
	(AttachParent(_classname, std::string(pulvis::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
}

//////////////////////////////////////////////////////////////////////////
#define RTTI_CLASS_API(class_t, ...)																												  \
public:																																										    \
	virtual pulvis::rtti::type_id_t GetTypeId() const override																  \
	{																																													  \
		return pulvis::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::GetTypeId();								  \
	}																																													  \
	virtual bool IsTypeOfId(pulvis::rtti::type_id_t _type) const override											  \
	{																																													  \
		return pulvis::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::IsTypeOfId(_type);					  \
	}																																													  \
	virtual void const* internal_cast(pulvis::rtti::type_id_t _type_id) const noexcept override \
	{																																													  \
		const pulvis::rtti::type_id_t type_id = pulvis::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::GetTypeId();	\
		if (type_id == _type_id)																																	\
		{																																												  \
			return pulvis::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::Cast(_type_id, this);				\
		}																																												  \
		return nullptr;																																						\
	}																																													  \
