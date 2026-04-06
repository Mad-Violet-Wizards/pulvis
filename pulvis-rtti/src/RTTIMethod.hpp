#pragma once

#include "FastFunction.hpp"
#include "RTTIAttributes.hpp"
#include <string>

namespace pulvis::rtti
{
	constexpr static const char* s_RttiInvalidMethod = "INVALID_METHOD";

	class CRTTIMethod
	{
	public:

		CRTTIMethod()
			: m_Name(s_RttiInvalidMethod)
			, m_Attributes(ERTTIMethodAttribute::None)
		{
		}

		template<typename R, class C, typename... Args>
		CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...), ERTTIMethodAttribute _attributes = ERTTIMethodAttribute::None)
			: m_Name(_method_name)
			, m_Function(_method)
			, m_Attributes(_attributes)
		{
		}

		template<typename R, class C, typename... Args>
		CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...) const, ERTTIMethodAttribute _attributes = ERTTIMethodAttribute::None)
			: m_Name(_method_name)
			, m_Function(_method)
			, m_Attributes(_attributes)
		{
		}

		bool HasAttribute(ERTTIMethodAttribute _attribute) const
		{
			return (m_Attributes & _attribute) == _attribute;
		}

		ERTTIMethodAttribute GetAttributes() const
		{
			return m_Attributes;
		}

		template<typename R, class C, typename... Args>
		R Invoke(C* _instance, Args&&... _args)
		{
			return m_Function.Invoke<R>(static_cast<C*>(_instance), std::forward<Args>(_args)...);
		}

		template<typename R, class C, typename... Args>
		R Invoke(C* _instance, Args&&... _args) const
		{
			return m_Function.Invoke<R>(static_cast<C*>(_instance), std::forward<Args>(_args)...);
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

	private:

		std::string m_Name;
		pulvis::tl::FastFunction m_Function;
		ERTTIMethodAttribute m_Attributes;
	};
}