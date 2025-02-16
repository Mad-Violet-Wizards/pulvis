#pragma once

#include "engine/rtti/RTTITypeName.hpp"

namespace engine::rtti::detail
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
	class PULVIS_API CRTTIEnumStorage
	{
		public:

			static void RegisterEnum(const SEnumDataBuffer& _enum_data_buffer);
			static SEnumDataBuffer& GetEnumDataBufferRef(int _index);
			static const SEnumDataBuffer& GetEnumDataBufferConstRef(int _index);

			static int GetCurrentIndex();
			static void IncrementIndex();

			static constexpr int inline s_CheckValuesLimit = 256;
			static constexpr int inline s_MapperBufferLimit = 2048;

		private:

			static inline int s_EnumMapperIndex = 0;
			static inline SEnumDataBuffer s_EnumDataStorage[s_MapperBufferLimit];
	};
//////////////////////////////////////////////////////////////////////////
		template<typename E>
		static int GetEnumCount()
		{
			const int index = GetEnumIndex<E>();

			if (index == -1)
				return -1;


			return CRTTIEnumStorage::GetEnumDataBufferConstRef(index).m_EnumData.size();
		}

		template<typename E>
		static std::string_view ToString(E _value)
		{
			const int index = GetEnumIndex<E>();

			for (const SEnumValues& data : CRTTIEnumStorage::GetEnumDataBufferConstRef(index).m_EnumData)
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
			
			for (const SEnumValues& data : CRTTIEnumStorage::GetEnumDataBufferConstRef(index).m_EnumData)
			{
				if (data.m_EnumValueStr == _value)
				{
					return static_cast<E>(data.m_EnumValueInt);
				}
			}

			return static_cast<E>(0);
		}

		template<typename E, E EnumValue>
		static constexpr void RegisterIfValid() noexcept
		{
			constexpr std::string_view function_signature = __FUNCSIG__;
			constexpr std::string_view enum_name = CRTTITypeName::GetTypename<E>();

			constexpr size_t start = function_signature.rfind(enum_name) + enum_name.size();

			if (start >= function_signature.size())
				return;

			if (function_signature[start] == ':' && function_signature[start + 1] == ':')
			{
				constexpr size_t enum_value_start = start + 2;
				constexpr size_t enum_value_end = function_signature.find(">(void)") - enum_value_start;

				std::string_view enum_value_str = function_signature.substr(enum_value_start, enum_value_end);
				constexpr int enum_value_int = static_cast<int>(EnumValue);

				SEnumDataBuffer& buffer = CRTTIEnumStorage::GetEnumDataBufferRef(CRTTIEnumStorage::GetCurrentIndex());

				if (buffer.m_Valid)
				{
					buffer.m_EnumData.push_back({ enum_value_str, enum_value_int });
					return;
				}

				SEnumDataBuffer new_buffer;
				new_buffer.m_EnumName = enum_name;
				new_buffer.m_EnumData.push_back({ enum_value_str, enum_value_int });
				new_buffer.m_Valid = true;
				CRTTIEnumStorage::RegisterEnum(new_buffer);
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
			RegisterEnumImpl<E>(std::make_integer_sequence<int, CRTTIEnumStorage::s_CheckValuesLimit>());
			CRTTIEnumStorage::IncrementIndex();
		}

		template<typename E>
		static int GetEnumIndex()
		{
			for (int i = 0; i < CRTTIEnumStorage::s_MapperBufferLimit; ++i)
			{
				if (CRTTITypeName::GetTypename<E>() == CRTTIEnumStorage::GetEnumDataBufferConstRef(i).m_EnumName)
				{
					return i;
				}
			}
			
			return -1;
		}
}