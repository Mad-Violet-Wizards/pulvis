#pragma once

#include "engine/pstd/MacroUtils.hpp"
#include "RTTITypeTraits.hpp"
#include "RTTITypeName.hpp"
#include "RTTITypeInfo.hpp"
#include "RTTIMethodInfo.hpp"
#include "RTTIMethod.hpp"
#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "detail/RTTIClassDetail.hpp"

#pragma warning(disable: 5030)

//////////////////////////////////////////////////////////////////////////
static void AttachParent(const char* _class_name, const char* _parent_name)
{
	engine::rtti::detail::CRTTIClassStorage::FindClassByName(_class_name)->AttachParent(engine::rtti::detail::CRTTIClassStorage::FindClassByName(_parent_name));
}

template<typename... Parents>
static void RegisterParents(const char* _classname)
{
	(AttachParent(_classname, std::string(engine::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
}

//////////////////////////////////////////////////////////////////////////
#define RTTI_CLASS_API(class_t, ...)																												  \
public:																																										    \
	virtual engine::rtti::type_id_t GetTypeId() const override																  \
	{																																													  \
		return engine::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::GetTypeId();								  \
	}																																													  \
	virtual bool IsTypeOfId(engine::rtti::type_id_t _type) const override											  \
	{																																													  \
		return engine::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::IsTypeOfId(_type);					  \
	}																																													  \
	virtual void const* internal_cast(engine::rtti::type_id_t _type_id) const noexcept override \
	{																																													  \
		const engine::rtti::type_id_t type_id = engine::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::GetTypeId();	\
		if (type_id == _type_id)																																	\
		{																																												  \
			return engine::rtti::CRTTITypeInfo<class_t, ##__VA_ARGS__>::Cast(_type_id, this);				\
		}																																												  \
		return nullptr;																																						\
	}																																													  \
//////////////////////////////////////////////////////////////////////////
#define RTTI_METHOD_API(class_t, method_t)
//////////////////////////////////////////////////////////////////////////
#define RTTI_FIELD_API(class_t, field_name)
//////////////////////////////////////////////////////////////////////////
#define RTTI_ENUM_API(enumName)
//////////////////////////////////////////////////////////////////////////
