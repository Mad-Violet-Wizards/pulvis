#pragma once

#include "EcsTypes.hpp"
#include "EntityHandle.hpp"

#include <unordered_map>
#include <string>
#include <functional>
#include <any>

namespace pulvis::ecs
{
	class CWorld;

	using component_factory_fn_t = std::function<void(CWorld&, SEntityHandle)>;

	struct SComponentBlueprint final
	{
		component_id_t                            ComponentId = INVALID_COMPONENT_ID;
		std::string                               TypeName;
		component_factory_fn_t                    Factory;
		std::unordered_map<std::string, std::any> Defaults;
	};
} // namespace pulvis::ecs