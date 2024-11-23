#pragma once

#include "engine/pstd/FastFunction.hpp"

namespace engine::rtti::detail
{
	//////////////////////////////////////////////////////////////////////////
	class IRTTIMethod
	{
	public:

		virtual ~IRTTIMethod() = default;
	};

	////////////////////////////////////////////////////////////////////////
	template<typename R, class C, typename... Args>
	class CRTTIMethod : public IRTTIMethod
	{
	public:

		explicit CRTTIMethod(R(C::* _method)(Args...))
			: m_Function(_method)
		{
		}

		R Invoke(C* _instance, Args&&... _args)
		{
			return m_Function(_instance, std::forward<Args>(_args)...);
		}

	private:

		pstd::FastFunction<R, Args...> m_Function;
	};
	////////////////////////////////////////////////////////////////////////
	template<typename R, class C, typename... Args>
	class CRTTIMethodConst: public IRTTIMethod
	{
	public:

		explicit CRTTIMethodConst(R(C::* _method)(Args...) const)
			: m_Function(_method)
		{
		}

		R Invoke(C* _instance, Args&&... _args) const
		{
			return m_Function(_instance, std::forward<Args>(_args)...);
		}

	private:

		pstd::FastFunction<R, Args...> m_Function;
	};
}