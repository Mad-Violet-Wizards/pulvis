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
		Command = 1 << 2,      // Method can be invoked as a console command
		Pure = 1 << 3,         // Method is pure virtual (interface method)
	};


	enum class ERTTIClassAttribute : uint16_t
	{
		None = 0,
		Serializable = 1 << 0,    // Class supports serialization round-trip
		Scriptable = 1 << 1,      // Class is exposed to scripting languages
		ScriptableHook = 1 << 2,  // Event class auto-bridged to scripts (subscribe / submit)
		Replicated = 1 << 3,      // Class participates in network replication
		EditorVisible = 1 << 4,   // Class shows up in editor browsers
		Component = 1 << 5,       // Class is an ECS component
		Abstract = 1 << 6,        // Class is abstract
	};

	inline constexpr ERTTIFieldAttribute operator|(ERTTIFieldAttribute lhs, ERTTIFieldAttribute rhs)
	{
		return static_cast<ERTTIFieldAttribute>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIMethodAttribute operator|(ERTTIMethodAttribute lhs, ERTTIMethodAttribute rhs)
	{
		return static_cast<ERTTIMethodAttribute>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIClassAttribute operator|(ERTTIClassAttribute lhs, ERTTIClassAttribute rhs)
	{
		return static_cast<ERTTIClassAttribute>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
	}

	inline constexpr ERTTIFieldAttribute operator&(ERTTIFieldAttribute lhs, ERTTIFieldAttribute rhs)
	{
		return static_cast<ERTTIFieldAttribute>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIMethodAttribute operator&(ERTTIMethodAttribute lhs, ERTTIMethodAttribute rhs)
	{
		return static_cast<ERTTIMethodAttribute>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	inline constexpr ERTTIClassAttribute operator&(ERTTIClassAttribute lhs, ERTTIClassAttribute rhs)
	{
		return static_cast<ERTTIClassAttribute>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs));
	}
}