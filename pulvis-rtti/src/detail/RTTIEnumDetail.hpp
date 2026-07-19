#pragma once

#include <vector>
#include <unordered_map>

#include "RTTITypeName.hpp"
#include "RTTITypeTraits.hpp"
#include "Hash.hpp"
#include "DynamicLibraryExport.hpp"

namespace pulvis::rtti::detail
{
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
	//////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251)
	class PULVIS_DLL_EXPORT CRTTIEnumStorage
	{
		public:

			static void RegisterEnum(pulvis::rtti::type_id_t _id, SEnumDataBuffer _enum_data_buffer);
			static SEnumDataBuffer* GetEnumData(pulvis::rtti::type_id_t _index);

			static constexpr int inline CHECK_VALUES_LIMIT = 256;

		private:

			static std::unordered_map<pulvis::rtti::type_id_t, SEnumDataBuffer> ENUMS;
	};
#pragma warning(pop)
//////////////////////////////////////////////////////////////////////////
	template<typename E>
	static int GetEnumCount()
	{
		constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();
		constexpr pulvis::rtti::type_id_t id = tl::hash::fnv1a<type_id_t>(enum_name);
		const SEnumDataBuffer* enum_data = CRTTIEnumStorage::GetEnumData(id);

		if (!enum_data)
		{
			return 0;
		}

		return static_cast<int>(enum_data->m_EnumData.size());
	}

	template<typename E>
	static std::string_view ToString(E _value)
	{
		constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();
		constexpr pulvis::rtti::type_id_t id = tl::hash::fnv1a<type_id_t>(enum_name);
		const SEnumDataBuffer* enum_data = CRTTIEnumStorage::GetEnumData(id);
		
		if (enum_data)
		{
			for (const SEnumValues& data : enum_data->m_EnumData)
			{
				if (data.m_EnumValueInt == static_cast<int>(_value))
				{
					return data.m_EnumValueStr;
				}
			}
		}

		return "INVALID";
	}

	template<typename E>
	static E FromString(std::string_view _value)
	{
		constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();
		constexpr pulvis::rtti::type_id_t id = tl::hash::fnv1a<type_id_t>(enum_name);
		const SEnumDataBuffer* enum_data = CRTTIEnumStorage::GetEnumData(id);

		if (enum_data)
		{
			for (const SEnumValues& data : enum_data->m_EnumData)
			{
				if (data.m_EnumValueStr == _value)
				{
					return static_cast<E>(data.m_EnumValueInt);
				}
			}
		}

		return static_cast<E>(0);
	}

	template<typename E, E EnumValue>
	static constexpr void RegisterIfValid() noexcept
	{
#if defined(WINDOWS_OS)
		constexpr std::string_view function_signature = __FUNCSIG__;
#elif defined(MAC_OS)
		constexpr std::string_view function_signature = __PRETTY_FUNCTION__;
#endif
		constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();
		constexpr pulvis::rtti::type_id_t id = tl::hash::fnv1a<type_id_t>(enum_name);

		constexpr size_t start = function_signature.rfind(enum_name) + enum_name.size();

		if (start >= function_signature.size())
			return;

		if (function_signature[start] == ':' && function_signature[start + 1] == ':')
		{
			constexpr size_t enum_value_start = start + 2;
			constexpr size_t enum_value_end = function_signature.find(">(void)") - enum_value_start;

			std::string_view enum_value_str = function_signature.substr(enum_value_start, enum_value_end);
			constexpr int enum_value_int = static_cast<int>(EnumValue);

			SEnumDataBuffer* buffer = CRTTIEnumStorage::GetEnumData(id);

			if (buffer)
			{
				buffer->m_EnumData.push_back({ enum_value_str, enum_value_int });
				return;
			}

			SEnumDataBuffer new_buffer;
			new_buffer.m_EnumName = enum_name;
			new_buffer.m_EnumData.push_back({ enum_value_str, enum_value_int });
			CRTTIEnumStorage::RegisterEnum(id, new_buffer);
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
		RegisterEnumImpl<E>(std::make_integer_sequence<int, CRTTIEnumStorage::CHECK_VALUES_LIMIT>());
	}
}