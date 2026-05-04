#pragma once

#include "EcsTypes.hpp"

#include <atomic>

namespace pulvis::ecs
{
  namespace detail
  {
    inline component_id_t NextComponentId()
    {
      static std::atomic<component_id_t> s_Counter{ 0 };
      return s_Counter.fetch_add(1, std::memory_order_relaxed);
    }
  } // namespace detail

  template<typename TComponent>
  [[nodiscard]] inline component_id_t GetComponentId()
  {
    static const component_id_t s_Id = detail::NextComponentId();
    return s_Id;
  }
} // namespace pulvis::ecs