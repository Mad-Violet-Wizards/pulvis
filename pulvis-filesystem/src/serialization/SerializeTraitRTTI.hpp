#pragma once

#include "SerializeTrait.hpp"
#include "SerializationArchive.hpp"

#include "RTTIBase.hpp"
#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "RTTIAttributes.hpp"

namespace pulvis::fs::serialization
{
	template<typename C>
	void SerializeRTTIField(CSerializationArchive& _archive, const pulvis::rtti::CRTTIField* _field, C* _instance)
	{
		using namespace pulvis::rtti;

		if (!_field->HasAttribute(ERTTIFieldAttribute::Serializable))
		{
			return;
		}

		if (_field->GetFieldAccess() == ERTTIFieldAccess::Pointer || _field->GetFieldAccess() == ERTTIFieldAccess::Reference)
		{
			return;
		}

		const char* field_name = _field->GetName().c_str();
		switch (_field->GetFieldType())
		{
			case ERTTIFieldType::Int: _archive.WriteInt(field_name, _field->Get<int>(_instance)); break;
			case ERTTIFieldType::Float: _archive.WriteFloat(field_name, static_cast<double>(_field->Get<float>(_instance))); break;
			case ERTTIFieldType::Double: _archive.WriteFloat(field_name, _field->Get<double>(_instance)); break;
			case ERTTIFieldType::Bool: _archive.WriteBool(field_name, _field->Get<bool>(_instance)); break;
			case ERTTIFieldType::Char: { char c = _field->Get<char>(_instance); _archive.WriteInt(field_name, c); break; }
			case ERTTIFieldType::String: _archive.WriteString(field_name, _field->Get<std::string>(_instance)); break;
			case ERTTIFieldType::Long:      _archive.WriteInt(field_name, _field->Get<long>(_instance)); break;
			case ERTTIFieldType::LongLong:  _archive.WriteInt(field_name, _field->Get<long long>(_instance)); break;
			case ERTTIFieldType::Uint:      _archive.WriteUInt(field_name, _field->Get<unsigned int>(_instance)); break;
			case ERTTIFieldType::Uint8:     _archive.WriteUInt(field_name, _field->Get<uint8_t>(_instance)); break;
			case ERTTIFieldType::Uint16:    _archive.WriteUInt(field_name, _field->Get<uint16_t>(_instance)); break;
			case ERTTIFieldType::Uint32:    _archive.WriteUInt(field_name, _field->Get<uint32_t>(_instance)); break;
			case ERTTIFieldType::Uint64:    _archive.WriteUInt(field_name, _field->Get<uint64_t>(_instance)); break;
			default: break;
		}
	}

	template<typename C>
	void DeserializeRTTIField(CSerializationArchive& _archive, const pulvis::rtti::CRTTIField* _field, C* _instance)
	{
		using namespace pulvis::rtti;

		if (!_field->HasAttribute(ERTTIFieldAttribute::Serializable))
		{
			return;
		}

		if (_field->GetFieldAccess() == ERTTIFieldAccess::Pointer ||
			_field->GetFieldAccess() == ERTTIFieldAccess::Reference)
		{
			return;
		}

		const char* name = _field->GetName().c_str();
		switch (_field->GetFieldType())
		{
			case ERTTIFieldType::Int: { int64_t v; _archive.ReadInt(name, v); _field->Set(_instance, static_cast<int>(v)); break; }
			case ERTTIFieldType::Float: { double v; _archive.ReadFloat(name, v); _field->Set(_instance, static_cast<float>(v)); break; }
			case ERTTIFieldType::Double: { double v; _archive.ReadFloat(name, v); _field->Set(_instance, v); break; }
			case ERTTIFieldType::Bool: { bool v; _archive.ReadBool(name, v); _field->Set(_instance, v); break; }
			case ERTTIFieldType::Char: { int64_t v; _archive.ReadInt(name, v); _field->Set(_instance, static_cast<char>(v)); break; }
			case ERTTIFieldType::String: { std::string v; _archive.ReadString(name, v); _field->Set(_instance, v); break; }
			case ERTTIFieldType::Long: { int64_t v; _archive.ReadInt(name, v); _field->Set(_instance, static_cast<long>(v)); break; }
			case ERTTIFieldType::LongLong: { int64_t v; _archive.ReadInt(name, v); _field->Set(_instance, static_cast<long long>(v)); break; }
			case ERTTIFieldType::Uint: { uint64_t v; _archive.ReadUInt(name, v); _field->Set(_instance, static_cast<unsigned int>(v)); break; }
			case ERTTIFieldType::Uint8: { uint64_t v; _archive.ReadUInt(name, v); _field->Set(_instance, static_cast<uint8_t>(v)); break; }
			case ERTTIFieldType::Uint16: { uint64_t v; _archive.ReadUInt(name, v); _field->Set(_instance, static_cast<uint16_t>(v)); break; }
			case ERTTIFieldType::Uint32: { uint64_t v; _archive.ReadUInt(name, v); _field->Set(_instance, static_cast<uint32_t>(v)); break; }
			case ERTTIFieldType::Uint64: { uint64_t v; _archive.ReadUInt(name, v); _field->Set(_instance, static_cast<uint64_t>(v)); break; }
			default: break;
		}
	}

	template<typename T>
	struct TSerializeTrait<T, std::enable_if_t<
		std::is_base_of_v<pulvis::rtti::IRTTIBase, T> && !std::is_enum_v<T>>>
	{
		static void Serialize(CSerializationArchive& _ar, const char* _name, const T& _value)
		{
			using namespace pulvis::rtti;

			const CRTTIClass* cls = CRTTIClass::FindConstInStorage(_value.GetTypeId());
			if (!cls)
				return;

			_ar.BeginObject(_name);
			_ar.WriteString("__type", cls->GetName());

			T* mutable_ptr = const_cast<T*>(&_value);
			for (const CRTTIField* field : cls->GetFields())
				SerializeRTTIField(_ar, field, mutable_ptr);

			_ar.EndObject();
		}

		static void Deserialize(CSerializationArchive& _ar, const char* _name, T& _out_value)
		{
			using namespace pulvis::rtti;

			_ar.BeginObject(_name);

			std::string type_name;
			_ar.ReadString("__type", type_name);

			const CRTTIClass* cls = CRTTIClass::FindConstInStorage(type_name.c_str());
			if (cls)
			{
				for (const CRTTIField* field : cls->GetFields())
					DeserializeRTTIField(_ar, field, &_out_value);
			}

			_ar.EndObject();
		}
	};
}