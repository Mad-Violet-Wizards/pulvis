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

//////////////////////////////////////////////////////////////////////////
namespace
{
	static void AttachParent(const char* _class_name, const char* _parent_name)
	{
		engine::rtti::detail::CRTTIClassStorage::FindClassByName(_class_name)->AttachParent(engine::rtti::detail::CRTTIClassStorage::FindClassByName(_parent_name));
	}

	template<typename... Parents>
	static void RegisterParents(const char* _classname)
	{
		(AttachParent(_classname, std::string(engine::rtti::CRTTITypeName::GetTypename<Parents>()).c_str()), ...);
	}
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
private:																																											\
  struct CONCATENATE(SClassRegistrar_, class_t)																							  \
	{																																													  \
		CONCATENATE(SClassRegistrar_, class_t)()																								  \
		{																																												  \
	      const std::string class_name_str { engine::rtti::CRTTITypeName::GetTypename<class_t>() };	  \
				static engine::rtti::CRTTIClass rtti_class(class_name_str.c_str());									  \
				engine::rtti::detail::CRTTIClassStorage::RegisterClass(&rtti_class);												\
				RegisterParents<__VA_ARGS__>(class_name_str.c_str());																  \
		}																																												  \
	};																																												  \
	inline static CONCATENATE(SClassRegistrar_, class_t) s_ClassRegistrar_##class_t{};				  \
//////////////////////////////////////////////////////////////////////////
#define RTTI_METHOD_API(class_t, method_t)																																																					\
private:																																																																						\
  inline static engine::rtti::CRTTIMethod CONCATENATE(s_Method_, CONCATENATE(class_t, method_t)) {#class_t, #method_t, &class_t::method_t};					\
	struct CONCATENATE(SMethodRegistrar_, CONCATENATE(class_t, method_t))																																							\
	{																																																																									\
		CONCATENATE(SMethodRegistrar_, CONCATENATE(class_t, method_t))()																																								\
		{																																																																								\
			const std::string class_name_str { engine::rtti::CRTTITypeName::GetTypename<class_t>() };																											\
			engine::rtti::CRTTIClass* rtti_class = engine::rtti::detail::CRTTIClassStorage::FindClassByName(class_name_str.c_str());														\
      if (rtti_class)																																																																\
			{																																																																							\
				auto* method = &CONCATENATE(s_Method_, CONCATENATE(class_t, method_t));																																			\
				rtti_class->AddMethod(method);																																																							\
			}																																																																							\
		}																																																																								\
	};																																																																								\
	inline static CONCATENATE(SMethodRegistrar_, CONCATENATE(class_t, method_t)) CONCATENATE(s_MethodRegistrar, CONCATENATE(class_t, method_t)) = {}; \
//////////////////////////////////////////////////////////////////////////
#define RTTI_FIELD_API(class_t, field_name)																																																						\
private:																																																																							\
	inline static engine::rtti::CRTTIField CONCATENATE(s_Field_, CONCATENATE(class_t, field_name)) {#field_name, &class_t::field_name};									\
	struct CONCATENATE(SFieldRegistrar_, CONCATENATE(class_t, field_name))																																							\
	{																																																																										\
		CONCATENATE(SFieldRegistrar_, CONCATENATE(class_t, field_name))()																																									\
		{																																																																									\
			const std::string class_name_str{ engine::rtti::CRTTITypeName::GetTypename<class_t>() };																												\
			engine::rtti::CRTTIClass* rtti_class = engine::rtti::detail::CRTTIClassStorage::FindClassByName(class_name_str.c_str());															\
			if (rtti_class)																																																																	\
			{																																																																								\
				auto* field = &CONCATENATE(s_Field_, CONCATENATE(class_t, field_name));																																				\
				rtti_class->AddField(field);																																																									\
			}																																																																								\
		}																																																																									\
	};																																																																									\
	inline static CONCATENATE(SFieldRegistrar_, CONCATENATE(class_t, field_name)) CONCATENATE(s_FieldRegistrar, CONCATENATE(class_t, field_name)) = {}; \
//////////////////////////////////////////////////////////////////////////
#define RTTI_ENUM_API(enumName)																					\
inline static engine::rtti::CRTTIEnum<enumName> s_Enum_##enumName = {}; \
//////////////////////////////////////////////////////////////////////////
