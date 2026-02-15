#pragma once
#include "RTTITypeInfo.hpp"

#include <string>
#include <memory>
#include <any>

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
			{
			}
			
			template<class C, typename T>
			CRTTIField(const std::string& _name, T(C::* _field))
				: m_Name(_name)
				, m_FieldPtr(_field)
			{
				m_FieldType = CRTTITypeInfo<T>::GetFieldType();
				m_FieldAccess = CRTTITypeInfo<T>::GetFieldAccess();
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

			std::string m_Name;
			std::any m_FieldPtr;
			ERTTIFieldType m_FieldType;
			ERTTIFieldAccess m_FieldAccess;

	};
}