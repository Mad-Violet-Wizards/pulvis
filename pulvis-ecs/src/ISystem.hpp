#pragma once

#include "SystemPhase.hpp"
#include "EcsTypes.hpp"

#include <bitset>
#include <string>

namespace pulvis::ecs
{
	class CWorld;

	class ISystem
	{
		public:

      virtual ~ISystem() = default;

      virtual void Frame(CWorld& _world, float _delta_time) = 0;

      [[nodiscard]] virtual ESystemPhase Phase() const = 0;
      [[nodiscard]] virtual const std::string& Name() const = 0;

      [[nodiscard]] virtual const std::bitset<MAX_COMPONENT_TYPES>& Reads() const = 0;
      [[nodiscard]] virtual const std::bitset<MAX_COMPONENT_TYPES>& Writes() const = 0;
      [[nodiscard]] virtual bool IsThreadSafe() const = 0;
	};
}