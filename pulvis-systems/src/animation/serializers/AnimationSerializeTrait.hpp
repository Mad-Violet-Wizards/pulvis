#pragma once

#include "serialization/SerializeTrait.hpp"

#include "animation/AnimationFrame.hpp"
#include "animation/AnimationEvent.hpp"
#include "animation/AnimationDesc.hpp"

namespace pulvis::fs::serialization
{
	template<> struct TSerializeTrait<pulvis::systems::animation::CAnimationFrame>
	{
		static void Serialize(CSerializationArchive&, const char*, const pulvis::systems::animation::CAnimationFrame&);
		static void Deserialize(CSerializationArchive&, const char*, pulvis::systems::animation::CAnimationFrame&);
	};
	template<> struct TSerializeTrait<pulvis::systems::animation::SAnimationEvent>
	{
		static void Serialize(CSerializationArchive&, const char*, const pulvis::systems::animation::SAnimationEvent&);
		static void Deserialize(CSerializationArchive&, const char*, pulvis::systems::animation::SAnimationEvent&);
	};
	template<> struct TSerializeTrait<pulvis::systems::animation::CAnimationSequenceEventDesc>
	{
		static void Serialize(CSerializationArchive&, const char*, const pulvis::systems::animation::CAnimationSequenceEventDesc&);
		static void Deserialize(CSerializationArchive&, const char*, pulvis::systems::animation::CAnimationSequenceEventDesc&);
	};

	// Composite authoring structs - scalar fields auto, nested containers explicit.
	template<> struct TSerializeTrait<pulvis::systems::animation::CAnimationSequenceDesc>
	{
		static void Serialize(CSerializationArchive&, const char*, const pulvis::systems::animation::CAnimationSequenceDesc&);
		static void Deserialize(CSerializationArchive&, const char*, pulvis::systems::animation::CAnimationSequenceDesc&);
	};
	template<> struct TSerializeTrait<pulvis::systems::animation::CAnimationDesc>
	{
		static void Serialize(CSerializationArchive&, const char*, const pulvis::systems::animation::CAnimationDesc&);
		static void Deserialize(CSerializationArchive&, const char*, pulvis::systems::animation::CAnimationDesc&);
	};
} // namespace pulvis::fs::serialization