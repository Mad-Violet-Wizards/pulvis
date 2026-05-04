#pragma once

#include "EntityHandle.hpp"
#include "ComponentTraits.hpp"

namespace pulvis::ecs
{
	class IComponentStorage
	{
		public:

      virtual ~IComponentStorage() = default;

			virtual void EmplaceDefault(SEntityHandle _entity) = 0;
      virtual void RemoveIfPresent(SEntityHandle _entity) = 0;

      [[nodiscard]] virtual bool Has(SEntityHandle _entity) const = 0;
      [[nodiscard]] virtual void* TryGetRaw(SEntityHandle _entity) = 0;
      [[nodiscard]] virtual size_t Count() const = 0;
      [[nodiscard]] virtual component_id_t GetTypeId() const = 0;
      [[nodiscard]] virtual SComponentTraits& Traits() = 0;
      [[nodiscard]] virtual const SComponentTraits& Traits() const = 0;
      virtual void Clear() = 0;
	};
}