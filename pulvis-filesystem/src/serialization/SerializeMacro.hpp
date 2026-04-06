#pragma once

#include "SerializeTrait.hpp"

/* 
	PULVIS_SERIALIZE - Macro for custom types not registered with RTTI.

	Defines TSerializeTrait<T> with both Serialize and Deserialize.
	The body receives:
		_ar  - CSerializationArchive reference.
		ref  - Mutable reference to the value.

	Example:
		struct SPlayerData { int Health; float Speed; std::string Name; };

		PULVIS_SERIALIZE(SPlayerData,
		{
				_ar.Process("health", ref.Health);
				_ar.Process("speed",  ref.Speed);
				_ar.Process("name",   ref.Name);
		})

	The macro wraps the body in BeginObject/EndObject so nested types work.
*/

#define PULVIS_SERIALIZE(Type, Body)                                           \
template<>                                                                     \
struct pulvis::fs::serialization::TSerializeTrait<Type>                                       \
{                                                                              \
    static void Serialize(::pulvis::fs::serialization::CSerializationArchive& _ar,            \
                          const char* _name, const Type& _value)               \
    {                                                                          \
        _ar.BeginObject(_name);                                                \
        Type& ref = const_cast<Type&>(_value);                                 \
        Body                                                                   \
        _ar.EndObject();                                                       \
    }                                                                          \
    static void Deserialize(::pulvis::fs::serialization::CSerializationArchive& _ar,          \
                            const char* _name, Type& _value)                   \
    {                                                                          \
        _ar.BeginObject(_name);                                                \
        Type& ref = _value;                                                    \
        Body                                                                   \
        _ar.EndObject();                                                       \
    }                                                                          \
};
