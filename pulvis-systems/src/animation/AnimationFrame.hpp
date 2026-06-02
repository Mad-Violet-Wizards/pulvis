#pragma once

#include <cstdint>

#include "RTTIBase.hpp"
#include "RTTIMacros.hpp"

namespace pulvis::systems::animation
{
	class [[Reflection::Class]] CAnimationFrame : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CAnimationFrame);

		public:

			[[Reflection::Field("Serializable")]]
			uint16_t X = 0; // [px]]

			[[Reflection::Field("Serializable")]]
			uint16_t Y = 0; // [px]]

			[[Reflection::Field("Serializable")]]
			uint16_t Width = 0; // [px]]

			[[Reflection::Field("Serializable")]]
			uint16_t Height = 0; // [px]]

			[[Reflection::Field("Serializable")]]
			float Duration = 0.f; // [ms]
	};
}
