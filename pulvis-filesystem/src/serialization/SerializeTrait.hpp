#pragma once

#include <type_traits>
#include <string>
#include <vector>

/*
	TSerializeTrait<T> - Compile-time dispatch for serializing type T.

	The default (unspecialized) template triggers a static_assert.
	Supported types must have an explicit specialization or match
	one of the SFINAE-enabled partial specializations.

	Resolution order:
		1. Signed integers (int, int64_t, ...) - excludes bool.
		2. Unsigned integers (uint32_t, uint64_t, ...) - excludes bool.
		3. Floating-point (float, double).
		4. bool.
		5. std::string.
		6. std::vector<T> (recursive).
		7. Enum types (via pulvis::rtti::CRTTIEnum - string representation).
		8. IRTTIBase-derived classes (via CRTTIClass::GetFields - automatic).
		9. Custom types (via PULVIS_SERIALIZE macro).
*/

namespace pulvis::fs::serialization
{
	class CSerializationArchive;

	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename Enable = void>
	struct TSerializeTrait
	{
		static_assert(sizeof(T) == 0, "Type T is not supported for serialization. Please provide a specialization of TSerializeTrait or use the PULVIS_SERIALIZE macro.");
	};

	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	struct TSerializeTrait<T, std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T> && !std::is_same_v<T, bool>>>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const T& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, T& _value);
	};

	template<typename T>
	struct TSerializeTrait<T, std::enable_if_t<
		std::is_integral_v<T>&& std::is_unsigned_v<T> && !std::is_same_v<T, bool>>>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const T& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value);
	};

	template<typename T>
	struct TSerializeTrait<T, std::enable_if_t<std::is_floating_point_v<T>>>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const T& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value);
	};

	template<>
	struct TSerializeTrait<bool>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const bool& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, bool& _out_value);
	};

	template<>
	struct TSerializeTrait<std::string>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const std::string& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, std::string& _out_value);
	};

	template<typename T>
	struct TSerializeTrait<std::vector<T>>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const std::vector<T>& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, std::vector<T>& _out_value);
	};

	template<typename T>
	struct TSerializeTrait<T, std::enable_if_t<std::is_enum_v<T>>>
	{
		static void Serialize(CSerializationArchive& _archive, const char* _name, const T& _value);
		static void Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value);
	};
}

