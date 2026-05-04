#pragma once
#include "RTTITypeInfo.hpp"
#include "RTTIAttributes.hpp"

#include <string>
#include <memory>
#include <any>
#include <functional>

namespace pulvis::rtti
{
	constexpr static const char* s_RttiInvalidField = "INVALID_FIELD";

	class CRTTIField
	{
	public:

		CRTTIField()
			: m_Name(s_RttiInvalidField)
			, m_FieldType(ERTTIFieldType::Unknown)
			, m_FieldAccess(ERTTIFieldAccess::Unknown)
			, m_Attributes(ERTTIFieldAttribute::None)
		{
		}

		template<class C, typename T>
		CRTTIField(const std::string& _name, T(C::* _field), ERTTIFieldAttribute _attributes = ERTTIFieldAttribute::None)
			: m_Name(_name)
			, m_FieldPtr(_field)
			, m_Attributes(_attributes)
		{
			m_FieldType = CRTTITypeInfo<T>::GetFieldType();
			m_FieldAccess = CRTTITypeInfo<T>::GetFieldAccess();

			// Bake type-erased accessors so callers that only have a void*
			// (e.g. script bridges, serializers) don't need to know C/T.
			m_SetByAny = [_field](void* _instance, const std::any& _value)
				{
					if (_instance == nullptr || !_value.has_value()) { return; }
					static_cast<C*>(_instance)->*_field = std::any_cast<T>(_value);
				};

			m_GetByAny = [_field](const void* _instance) -> std::any
				{
					if (_instance == nullptr) { return {}; }
					return std::any(static_cast<const C*>(_instance)->*_field);
				};
		}

		template<class C, typename T>
		void Set(C* _instance, T _value) const
		{
			auto field = std::any_cast<T C::*>(m_FieldPtr);
			_instance->*field = _value;
		}

		template<class C, typename T>
		void GetByRef(C* _instance, T& _out_val) const
		{
			auto field = std::any_cast<T C::*>(m_FieldPtr);
			_out_val = _instance->*field;
		}

		template<typename T, class C>
		T Get(C* _instance) const
		{
			auto field = std::any_cast<T C::*>(m_FieldPtr);
			return _instance->*field;
		}

		// Type-erased counterparts. _instance must point to an object of the
		// class the field was registered with (or its address must coincide
		// with it - true for single-inheritance polymorphic types where the
		// base subobject sits at offset 0).
		void SetByAny(void* _instance, const std::any& _value) const
		{
			if (m_SetByAny) { m_SetByAny(_instance, _value); }
		}

		[[nodiscard]] std::any GetByAny(const void* _instance) const
		{
			return m_GetByAny ? m_GetByAny(_instance) : std::any{};
		}

		bool HasAttribute(ERTTIFieldAttribute _attribute) const
		{
			return (m_Attributes & _attribute) == _attribute;
		}

		ERTTIFieldAttribute GetAttributes() const
		{
			return m_Attributes;
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

		ERTTIFieldType GetFieldType() const
		{
			return m_FieldType;
		}

		ERTTIFieldAccess GetFieldAccess() const
		{
			return m_FieldAccess;
		}

	private:

		std::string                                          m_Name;
		std::any                                             m_FieldPtr;
		ERTTIFieldType                                       m_FieldType;
		ERTTIFieldAccess                                     m_FieldAccess;
		ERTTIFieldAttribute                                  m_Attributes;
		std::function<void(void*, const std::any&)>          m_SetByAny;
		std::function<std::any(const void*)>                 m_GetByAny;
	};
} // namespace pulvis::rtti