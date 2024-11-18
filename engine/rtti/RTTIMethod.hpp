#pragma once

#include <functional>
#include <any>

#include "detail/RTTIMethodDetail.hpp"
#include "RTTIMethodInfo.hpp"

namespace engine::rtti
{
	constexpr static const char* s_RttiInvalidMethod = "INVALID_METHOD";

	class PULVIS_API CRTTIMethod
	{
		public:

			CRTTIMethod()
				: m_MethodName(s_RttiInvalidMethod)
				, m_MethodImpl(nullptr)
			{

			}

			template<typename R, class C, typename... Args>
			CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...))
				: m_MethodName(_method_name)
			{
				m_MethodImpl = std::make_unique<detail::CRTTIMethod<R, C, Args...>>(_method);
			}

			template<typename R, class C, typename... Args>
			CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...) const)
				: m_MethodName(_method_name)
			{
				m_MethodImpl = std::make_unique<detail::CRTTIMethodConst<R, C, Args...>>(_method);
			}

			const std::string& GetName() const
			{
				return m_MethodName;
			}

			template<typename R, class C, typename... Args>
			R Invoke(C* _instance, Args... _args)
			{
				auto* method = static_cast<detail::CRTTIMethod<R, C, Args...>*>(m_MethodImpl.get());
				return method->Invoke(_instance, std::forward<Args>(_args)...);
			}

			template<typename R, class C, typename... Args>
			R Invoke(C* _instance, Args... _args) const
			{
				auto* method = static_cast<detail::CRTTIMethodConst<R, C, Args...>*>(m_MethodImpl.get());
				return method->Invoke(_instance, std::forward<Args>(_args)...);
			}


		private:

			std::string m_MethodName;
			std::unique_ptr<detail::IRTTIMethod> m_MethodImpl;
	};
}