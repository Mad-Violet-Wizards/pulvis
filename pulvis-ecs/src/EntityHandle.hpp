#pragma once

#include "EcsTypes.hpp"

#include <functional>

namespace pulvis::ecs
{
  struct SEntityHandle final
  {
    entity_index_t      Index = INVALID_ENTITY_INDEX;
    entity_generation_t Generation = INVALID_ENTITY_GENERATION;

    [[nodiscard]] bool IsValid() const
    {
      return Index != INVALID_ENTITY_INDEX && Generation != INVALID_ENTITY_GENERATION;
    }

    [[nodiscard]] bool operator==(const SEntityHandle& _other) const
    {
      return Index == _other.Index && Generation == _other.Generation;
    }

    [[nodiscard]] bool operator!=(const SEntityHandle& _other) const
    {
      return !(*this == _other);
    }
  };
} // namespace pulvis::ecs

namespace std
{
  template<>
  struct hash<pulvis::ecs::SEntityHandle>
  {
    size_t operator()(const pulvis::ecs::SEntityHandle& _h) const noexcept
    {
      return (static_cast<size_t>(_h.Generation) << 32) ^ _h.Index;
    }
  };
} // namespace std