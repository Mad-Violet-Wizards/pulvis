#pragma once

#include "FastFunction.hpp"
#include <string>

namespace pulvis::rtti
{
	constexpr static const char* s_RttiInvalidMethod = "INVALID_METHOD";

	class CRTTIMethod
	{
	public:

		CRTTIMethod()
			: m_Name(s_RttiInvalidMethod)
		{
		}

		template<typename R, class C, typename... Args>
		CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...))
			: m_Name(_method_name)
			, m_Function(_method)
		{
		}

		template<typename R, class C, typename... Args>
		CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...) const)
			: m_Name(_method_name)
			, m_Function(_method)
		{
		}

		const std::string& GetName() const
		{
			return m_Name;
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

	private:

		std::string m_Name;
		pulvis::tl::FastFunction m_Function;
	};
}