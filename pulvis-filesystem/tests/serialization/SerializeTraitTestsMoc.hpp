#pragma once

#include <cstdint>
#include "serialization/SerializeMacro.hpp"
#include "serialization/SerializationArchive.hpp"
#include "RTTIMacros.hpp"

using namespace pulvis::fs;
using namespace pulvis::fs::serialization;

namespace pulvis::fs::tests
{
	struct SPlayerData
	{
		int Health = 0;
		float Speed = 0.0f;
		std::string Name;
	};

	struct SInventoryItem
	{
		int32_t Id = 0;
		std::string Label;
		std::vector<int32_t> Tags;
	};

	enum class [[Reflection::Enum]] ETestElement : int 
	{
		Fire = 0,
		Water = 1,
		Earth = 2,
		Air = 3
	};

	class [[Reflection::Class]] CTestComponent : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CTestComponent);

	public:

		[[Reflection::Field]]
		int m_Health = 0;

		[[Reflection::Field]]
		float m_Speed = 0.0f;

		[[Reflection::Field]]
		bool m_Active = false;

		[[Reflection::Field]]
		double m_DamageMultiplier = 0.0;

		[[Reflection::Field]]
		uint32_t m_Level = 0;

		[[Reflection::Field]]
		std::string m_Name;
	};

	class [[Reflection::Class]] CMinimalComponent : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CMinimalComponent);

	public:

		[[Reflection::Field]]
		int m_Value = 0;
	};
}

PULVIS_SERIALIZE(pulvis::fs::tests::SPlayerData,
{
		_ar.Process("health", ref.Health);
		_ar.Process("speed", ref.Speed);
		_ar.Process("name", ref.Name);
})

PULVIS_SERIALIZE(pulvis::fs::tests::SInventoryItem,
{
	_ar.Process("id", ref.Id);
	_ar.Process("label", ref.Label);
	_ar.Process("tags", ref.Tags);
})