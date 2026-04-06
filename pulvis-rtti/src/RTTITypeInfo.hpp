#pragma once

#include "RTTITypeTraits.hpp"
#include "RTTITypeName.hpp"
#include "Hash.hpp"

namespace pulvis::rtti
{
	class IRTTIBase;

	enum class ERTTIFieldType
	{
		Unknown,
		Int,
		Float,
		Double,
		Bool,
		Char,
		String,
		Long,
		LongLong,
		Uint,
		Uint8,
		Uint16,
		Uint32,
		Uint64
	};

	enum class ERTTIFieldAccess
	{
		Unknown,
		Value,
		Pointer,
		Reference
	};

	template<typename T, typename... Parents>
	class CRTTITypeInfo
	{
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
				return pulvis::tl::hash::fnv1a(GetTypeName());
			}

			static constexpr ERTTIFieldType GetFieldType() noexcept
			{
				if (GetFieldAccess() == ERTTIFieldAccess::Pointer)
				{
					using raw_t = std::remove_pointer_t<T>;
					return GetFieldTypeInternal<raw_t>();
				}

				if (GetFieldAccess() == ERTTIFieldAccess::Reference)
				{
					using raw_t = std::remove_reference_t<T>;
					return GetFieldTypeInternal<raw_t>();
				}

				return GetFieldTypeInternal<T>();
			}

			static constexpr ERTTIFieldAccess GetFieldAccess() noexcept
			{
				if (std::is_pointer<T>::value)
				{
					return ERTTIFieldAccess::Pointer;
				}

				if (std::is_reference<T>::value)
				{
					return ERTTIFieldAccess::Reference;
				}

				if (std::is_fundamental<T>::value)
				{
					return ERTTIFieldAccess::Value;
				}

				return ERTTIFieldAccess::Unknown;
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

			template<typename U>
			static void const* Cast(type_id_t _type_id, U const* ptr) noexcept
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

			private:
				
				template<typename U>
				static constexpr ERTTIFieldType GetFieldTypeInternal()
				{

					if (std::is_same_v<U, int>)
					{
						return ERTTIFieldType::Int;
					}

					if (std::is_same_v<U, float>)
					{
						return ERTTIFieldType::Float;
					}

					if (std::is_same_v<U, double>)
					{
						return ERTTIFieldType::Double;
					}

					if (std::is_same_v<U, bool>)
					{
						return ERTTIFieldType::Bool;
					}

					if (std::is_same_v<U, char>)
					{
						return ERTTIFieldType::Char;
					}

					if (std::is_same_v<U, std::string>)
					{
						return ERTTIFieldType::String;
					}

					if (std::is_same_v<U, long>)
					{
						return ERTTIFieldType::Long;
					}

					if (std::is_same_v<U, long long>)
					{
						return ERTTIFieldType::LongLong;
					}

					if (std::is_same_v<U, uint8_t>)
					{
						return ERTTIFieldType::Uint8;
					}

					if (std::is_same_v<U, uint16_t>)
					{
						return ERTTIFieldType::Uint16;
					}

					if (std::is_same_v<U, uint32_t>)
					{
						return ERTTIFieldType::Uint32;
					}

					if (std::is_same_v<U, uint64_t>)
					{
						return ERTTIFieldType::Uint64;
					}

					if (std::is_same_v<U, unsigned int>)
					{
						return ERTTIFieldType::Uint;
					}

					return ERTTIFieldType::Unknown;
				}

	};
}