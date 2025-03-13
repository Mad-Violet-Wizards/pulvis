#pragma once

#include "RTTITypeTraits.hpp"
#include "RTTITypeName.hpp"
#include "RTTIHash.hpp"

namespace engine::rtti
{
	class IRTTIBase;

	template<typename Type, typename... Parents>
	class CRTTITypeInfo
	{
		using T = std::decay_t<Type>;

		static_assert((... && std::is_base_of<Parents, T>::value),
			"One or more parents are not a base of this type.");

		static_assert((... && std::is_base_of<IRTTIBase, Parents>::value),
			"Missing IRTTIBase in hierarchy.");

		public:

			static constexpr std::string_view GetTypeName() noexcept
			{
				return CRTTITypeName::GetTypename<T>();
			}

			static constexpr std::string_view GetScopedTypeName() noexcept
			{
				return CRTTITypeName::template GetScopedTypename<T>();
			}

			static constexpr type_id_t GetTypeId() noexcept
			{
				return Hash(GetTypeName().data());
			}

			static constexpr bool IsTypeOfId(type_id_t _type) noexcept
			{
				if (GetTypeId() == _type)
				{
					return true;
				}
				
				if constexpr (sizeof...(Parents) > 0)
				{
					return (... || CRTTITypeInfo<Parents>::IsTypeOfId(_type));
				}

				return false;
			}

			template<typename T>
			static void const* Cast(type_id_t _type_id, T const* ptr) noexcept
			{
				if (GetTypeId() == _type_id)
				{
					return static_cast<T const*>(ptr);
				}

				if constexpr (sizeof...(Parents) > 0)
				{
					(... || CRTTITypeInfo<Parents>::Cast(_type_id, ptr));
				}

				return nullptr;
			}
	};
}