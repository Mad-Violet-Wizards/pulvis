#pragma once
#include "detail/RTTIFieldDetail.hpp"

namespace engine::rtti
{
	constexpr static const char* s_RttiInvalidField = "INVALID_FIELD";

	class CRTTIField
	{
		public:

			CRTTIField()
				: m_Name(s_RttiInvalidField)
				, m_FieldImpl(nullptr)
			{
			}
			
			template<class C, typename T>
			CRTTIField(const std::string& _name, T(C::* _field))
				: m_Name(_name)
			{
				m_FieldImpl = std::make_unique<detail::RTTIField<C, T>>(_field);
			}

			template<class C, typename T>
			void Set(C* _instance, T _value) const
			{
				auto* field = static_cast<detail::RTTIField<C, T>*>(m_FieldImpl.get());
				field->Set(_instance, _value);
			}

			template<class C, typename T>
			void GetByRef(C* _instance, T& _out_val) const
			{
				auto* field = static_cast<detail::RTTIField<C, T>*>(m_FieldImpl.get());
				_out_val = field->Get(_instance);
			}

			template<typename T, class C>
			T Get(C* _instance) const
			{
				auto* field = static_cast<detail::RTTIField<C, T>*>(m_FieldImpl.get());
				return field->Get(_instance);
			}

			const std::string& GetName() const
			{
				return m_Name;
			}

		private:

			std::string m_Name;
			std::unique_ptr<detail::IRTTIField> m_FieldImpl;
	};
}