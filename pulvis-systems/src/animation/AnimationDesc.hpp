#pragma once

#include "AnimationFrame.hpp"

#include "RTTIBase.hpp"
#include "RTTIMacros.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace pulvis::systems::animation
{
	class [[Reflection::Class]] CAnimationSequenceEventDesc final : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CAnimationSequenceEventDesc);

		public:

			[[Reflection::Field("Serializable")]]
			std::string AnimEventName;

			[[Reflection::Field("Serializable")]]
			uint16_t    Frame = 0;
	};

	class [[Reflection::Class]] CAnimationSequenceDesc final : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CAnimationSequenceDesc);
		
		public:

			[[Reflection::Field("Serializable")]]
			std::string Name;

			[[Reflection::Field("Serializable")]]
			std::string Playback = "Looped";

			[[Reflection::Field("Serializable")]]
			std::vector<CAnimationFrame> Frames;

			[[Reflection::Field("Serializable")]]
			std::vector<CAnimationSequenceEventDesc> Events;
	};

	class [[Reflection::Class]] CAnimationDesc final : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CAnimationDesc);

		public:

		[[Reflection::Field("Serializable")]] 
		std::string Name;

		[[Reflection::Field("Serializable")]]
		std::string TexturePath;

		[[Reflection::Field("Serializable")]]
		std::vector<CAnimationSequenceDesc> Sequences;
	};
} // namespace pulvis::systems::animation