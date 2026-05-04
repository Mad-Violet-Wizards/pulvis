#include "SignalTypes.hpp"

#include "Hash.hpp"

namespace pulvis::ecs
{
  signal_id_t HashSignalName(std::string_view _name) noexcept
  {
		return pulvis::tl::hash::fnv1a(_name);
  }
} // namespace pulvis::ecs