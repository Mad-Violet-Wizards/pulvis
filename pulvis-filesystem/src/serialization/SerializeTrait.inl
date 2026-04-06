#pragma once

#include "SerializeTrait.hpp"
#include "SerializationArchive.hpp"
#include "RTTIEnum.hpp"

namespace pulvis::fs::serialization
{
	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T> && !std::is_same_v<T, bool>>>::Serialize(CSerializationArchive& _archive, const char* _name, const T& _value)
	{
		_archive.WriteInt(_name, static_cast<int64_t>(_value));
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T> && !std::is_same_v<T, bool>>>
		::Deserialize(CSerializationArchive& _archive, const char* _name, T& _value)
	{
		int64_t v = 0;
		_archive.ReadInt(_name, v);
		_value = static_cast<T>(v);
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T> && !std::is_same_v<T, bool>>>
		::Serialize(CSerializationArchive& _archive, const char* _name, const T& _value)
	{
		_archive.WriteUInt(_name, static_cast<uint64_t>(_value));
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T> && !std::is_same_v<T, bool>>>
		::Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value)
	{
		uint64_t v = 0;
		_archive.ReadUInt(_name, v);
		_out_value = static_cast<T>(v);
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_floating_point_v<T>>>
		::Serialize(CSerializationArchive& _archive, const char* _name, const T& _value)
	{
		_archive.WriteFloat(_name, static_cast<double>(_value));
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_floating_point_v<T>>>
		::Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value)
	{
		double v = 0.0;
		_archive.ReadFloat(_name, v);
		_out_value = static_cast<T>(v);
	}

	inline void TSerializeTrait<bool>::Serialize(CSerializationArchive& _archive, const char* _name, const bool& _value)
	{
		_archive.WriteBool(_name, _value);
	}

	inline void TSerializeTrait<bool>::Deserialize(CSerializationArchive& _archive, const char* _name, bool& _out_value)
	{
		_archive.ReadBool(_name, _out_value);
	}

	inline void TSerializeTrait<std::string>::Serialize(CSerializationArchive& _archive, const char* _name, const std::string& _value)
	{
		_archive.WriteString(_name, _value);
	}

	inline void TSerializeTrait<std::string>::Deserialize(CSerializationArchive& _archive, const char* _name, std::string& _out_value)
	{
		_archive.ReadString(_name, _out_value);
	}

	template<typename T>
	void TSerializeTrait<std::vector<T>>
		::Serialize(CSerializationArchive& _archive, const char* _name, const std::vector<T>& _value)
	{
		uint32_t count = static_cast<uint32_t>(_value.size());
		_archive.BeginArray(_name, count, true);
		for (uint32_t i = 0; i < count; ++i)
			TSerializeTrait<T>::Serialize(_archive, nullptr, _value[i]);
		_archive.EndArray();
	}

	template<typename T>
	void TSerializeTrait<std::vector<T>>
		::Deserialize(CSerializationArchive& _archive, const char* _name, std::vector<T>& _out_value)
	{
		uint32_t count = 0;
		_archive.BeginArray(_name, count, false);
		_out_value.resize(count);
		for (uint32_t i = 0; i < count; ++i)
			TSerializeTrait<T>::Deserialize(_archive, nullptr, _out_value[i]);
		_archive.EndArray();
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_enum_v<T>>>
		::Serialize(CSerializationArchive& _archive, const char* _name, const T& _value)
	{
		std::string str(pulvis::rtti::template CRTTIEnum<T>::ToString(_value));
		TSerializeTrait<std::string>::Serialize(_archive, _name, str);
	}

	template<typename T>
	void TSerializeTrait<T, std::enable_if_t<std::is_enum_v<T>>>
		::Deserialize(CSerializationArchive& _archive, const char* _name, T& _out_value)
	{
		std::string str;
		TSerializeTrait<std::string>::Deserialize(_archive, _name, str);
		_out_value = pulvis::rtti::template CRTTIEnum<T>::FromString(str);
	}
}
