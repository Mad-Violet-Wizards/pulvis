#pragma once

#include <type_traits>
#include "detail/RTTIEnumDetail.hpp"
#include "RTTITypeName.hpp"

namespace engine::rtti
{
	template<typename E>
	class CRTTIEnum
	{
		public:
	  
			constexpr CRTTIEnum()
			{
				static_assert(std::is_enum_v<E>, "RTTIEnum can only be used with enum types");
				engine::rtti::detail::enum_detail::RegisterEnum<E>();
			}

		public:

			inline static constexpr std::string_view GetName()
			{
				return CRTTITypeName::GetTypename<E>();
			}

			inline static int GetCount()
			{
				return detail::enum_detail::GetEnumCount<E>();
			}

			inline static std::string_view ToString(E _value)
			{
				return detail::enum_detail::ToString<E>(_value);
			}

			inline static E FromString(std::string_view _value)
			{
				return detail::enum_detail::FromString<E>(_value);
			}
	};
}