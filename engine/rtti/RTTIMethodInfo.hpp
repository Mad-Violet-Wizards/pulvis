#pragma once

namespace engine::rtti
{
	template<typename Method>
	class CRTTIMethodInfo;

	template<typename R, class C, typename... Args>
	class CRTTIMethodInfo<R(C::*)(Args...)>
	{
		public:
			static constexpr size_t ArgCount = sizeof...(Args);

			template<size_t Index>
			struct Arg
			{
				using type = std::tuple_element_t<Index, std::tuple<Args...>>::type;
			};
	};
}