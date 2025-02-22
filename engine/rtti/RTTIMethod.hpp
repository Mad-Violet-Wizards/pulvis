#pragma once

#include <functional>
#include <any>

#include "detail/RTTIMethodDetail.hpp"
#include "RTTIMethodInfo.hpp"

namespace engine::rtti
{
	constexpr static const char* s_RttiInvalidMethod = "INVALID_METHOD";

	class CRTTIMethod
	{
		public:

			CRTTIMethod()
			{
				m_Impl = nullptr;
			}

			~CRTTIMethod()
			{
				delete m_Impl;
			}

			template<typename R, class C, typename... Args>
			CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...))
			{
				m_Impl = new detail::CRTTIMethod<R, C, Args...>(_method);
				m_Impl->SetMethodName(_method_name);
			}

			template<typename R, class C, typename... Args>
			CRTTIMethod(const std::string& _class_name, const std::string& _method_name, R(C::* _method)(Args...) const)
			{
				m_Impl = new detail::CRTTIMethodConst<R, C, Args...>(_method);
				m_Impl->SetMethodName(_method_name);
			}

			const std::string& GetName() const
			{
				return m_Impl->GetMethodName();
			}

			template<typename R, class C, typename... Args>
			R Invoke(C* _instance, Args... _args)
			{
				auto* method = static_cast<detail::CRTTIMethod<R, C, Args...>*>(m_Impl);
				return method->Invoke(_instance, std::forward<Args>(_args)...);
			}

			template<typename R, class C, typename... Args>
			R Invoke(C* _instance, Args... _args) const
			{
				auto* method = static_cast<detail::CRTTIMethodConst<R, C, Args...>*>(m_Impl);
				return method->Invoke(_instance, std::forward<Args>(_args)...);
			}


		private:

			detail::IRTTIMethod* m_Impl;
	};
}