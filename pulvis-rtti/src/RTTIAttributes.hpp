#pragma once


#include <cstdint>
#include <type_traits>

namespace pulvis::rtti
{
	enum class ERTTIFieldAttribute : uint8_t
	{
		None = 0,
		Serializable = 1 << 0, // Field should be included in serialization
		Scriptable = 1 << 1,   // Field should be exposed to scripting languages
		Editable = 1 << 2,     // Field should be editable in editors
		EditorReadOnly = 1 << 3,   // Field should be read-only in editors
		Replicated = 1 << 4,    // Field should be replicated over the network
	};

	enum class ERTTIMethodAttribute : uint8_t
	{
		None = 0,
		Scriptable = 1 << 0,   // Method should be exposed to scripting languages
		EditorCallable = 1 << 1,   // Method can be called from editor UI (e.g. button click)
		Event = 1 << 2,        // Method is an event handler
		Signal = 1 << 3,       // Method is a signal that can be connected to slots
		Slot = 1 << 4,         // Method is a slot that can be connected to signals
		Command = 1 << 5,      // Method can be invoked as a console command
		Pure = 1 << 6,         // Method is pure virtual (interface method)
	};

	inline constexpr ERTTIFieldAttribute operator|(ERTTIFieldAttribute lhs, ERTTIFieldAttribute rhs)
	{
		return static_cast<ERTTIFieldAttribute>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIMethodAttribute operator|(ERTTIMethodAttribute lhs, ERTTIMethodAttribute rhs)
	{
		return static_cast<ERTTIMethodAttribute>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIFieldAttribute operator&(ERTTIFieldAttribute lhs, ERTTIFieldAttribute rhs)
	{
		return static_cast<ERTTIFieldAttribute>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIMethodAttribute operator&(ERTTIMethodAttribute lhs, ERTTIMethodAttribute rhs)
	{
		return static_cast<ERTTIMethodAttribute>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}
}