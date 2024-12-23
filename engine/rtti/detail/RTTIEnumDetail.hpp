#pragma once

#include "engine/rtti/RTTITypeName.hpp"

namespace engine::rtti::detail
{
//////////////////////////////////////////////////////////////////////////
	namespace enum_detail
	{
		namespace
		{
			static constexpr int s_CheckValuesLimit = 256;
			static constexpr int s_MapperBufferLimit = 2048;
			static int s_EnumMapperIndex = 0;

			struct SEnumValues
			{
				SEnumValues(std::string_view _enumValueStr, int _enumValueInt)
					: m_EnumValueStr(_enumValueStr)
					, m_EnumValueInt(_enumValueInt)
				{}

				std::string_view m_EnumValueStr;
				int m_EnumValueInt;
			};

			struct SEnumDataBuffer
			{
				std::string_view m_EnumName;
				std::vector<SEnumValues> m_EnumData;
				bool m_Valid = false;
			};
		}
		static inline std::array<SEnumDataBuffer, s_MapperBufferLimit> s_EnumDataStorage = {};
//////////////////////////////////////////////////////////////////////////
		template<typename E>
		static int GetEnumIndex()
		{
			for (int i = 0; i < s_MapperBufferLimit; ++i)
			{
				if (CRTTITypeName::GetTypename<E>() == s_EnumDataStorage[i].m_EnumName)
				{
					return i;
				}
			}
			
			return -1;
		}

		template<typename E>
		static int GetEnumCount()
		{
			const int index = GetEnumIndex<E>();

			if (index == -1)
				return -1;

			return s_EnumDataStorage[index].m_EnumData.size();
		}

		template<typename E>
		static std::string_view ToString(E _value)
		{
			const int index = GetEnumIndex<E>();

			for (const SEnumValues& data : s_EnumDataStorage[index].m_EnumData)
			{
				if (data.m_EnumValueInt == static_cast<int>(_value))
				{
					return data.m_EnumValueStr;
				}
			}

			return "INVALID";
		}

		template<typename E>
		static E FromString(std::string_view _value)
		{
			const int index = GetEnumIndex<E>();
			
			for (const SEnumValues& data : s_EnumDataStorage[index].m_EnumData)
			{
				if (data.m_EnumValueStr == _value)
				{
					return static_cast<E>(data.m_EnumValueInt);
				}
			}

			return static_cast<E>(0);
		}

#if defined(WINDOWS_OS)
		static constexpr std::string_view DetermineEnumValueStr(std::string_view _function_signature, std::string_view _enum_name)
		{
			constexpr size_t start = _function_signature.rfind(_enum_name) + _enum_name.size();

			if (_function_signature[start] == ':' && _function_signature[start + 1] == ':')
			{
				constexpr size_t enum_value_start = start + 2;
				constexpr size_t enum_value_end = _function_signature.find(">(void)") - enum_value_start;

				return _function_signature.substr(enum_value_start, enum_value_end);
			}

			return "";
		}
#else
		static constexpr std::string_view DetermineEnumValueStr(std::string_view _function_signature, std::string_view _enum_name)
		{
			return "";
		}
#endif

		template<typename E, E EnumValue>
		static constexpr void RegisterIfValid() noexcept
		{
#if defined(WINDOWS_OS)
			constexpr std::string_view function_signature = __FUNCSIG__;
#else
			constexpr std::string_view function_signature = __PRETTY_FUNCTION__;
#endif
			constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();
			constexpr std::string_view enum_value_str = DetermineEnumValueStr(function_signature, enum_name);

			if (!enum_value_str.empty())
			{
				constexpr int enum_value_int = static_cast<int>(EnumValue);

				if (s_EnumDataStorage[s_EnumMapperIndex].m_Valid)
				{
					SEnumDataBuffer& buffer = s_EnumDataStorage[s_EnumMapperIndex];
					buffer = s_EnumDataStorage[s_EnumMapperIndex];
					buffer.m_EnumData.push_back({ enum_value_str, enum_value_int });
					return;
				}

				SEnumDataBuffer buffer;
				buffer.m_EnumName = enum_name;
				buffer.m_EnumData.push_back({ enum_value_str, enum_value_int });
				buffer.m_Valid = true;
				s_EnumDataStorage[s_EnumMapperIndex] = buffer;
			}
		}

		template<typename E, int... I>
		static constexpr void RegisterEnumImpl(std::integer_sequence<int, I...> _values_to_check)
		{
			return (RegisterIfValid<E, (E)I>(), ...);
		}

		template<typename E>
		static constexpr void RegisterEnum()
		{
			RegisterEnumImpl<E>(std::make_integer_sequence<int, s_CheckValuesLimit>());
			s_EnumMapperIndex++;
			static_assert(s_EnumDataStorage.size() == s_MapperBufferLimit, "Enum buffer limit reached!");
		}
	};
}