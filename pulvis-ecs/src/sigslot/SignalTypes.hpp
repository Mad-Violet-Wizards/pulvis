#pragma once

#include "EntityHandle.hpp"
#include "ComponentTraits.hpp"

#include <any>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace pulvis::ecs
{
	using signal_id_t = uint32_t;
	using slot_id_t = uint64_t;

	static constexpr signal_id_t INVALID_SIGNAL_ID = std::numeric_limits<signal_id_t>::max();
	static constexpr slot_id_t   INVALID_SLOT_ID = std::numeric_limits<slot_id_t>::max();

	[[nodiscard]] signal_id_t HashSignalName(std::string_view _name) noexcept;

	struct SSignalConnection final
	{
		SEntityHandle Source;
		component_id_t SourceComponentId = INVALID_COMPONENT_ID;
		signal_id_t SignalId = INVALID_SIGNAL_ID;
		slot_id_t SlotId = INVALID_SLOT_ID;
    
    [[nodiscard]] bool IsValid() const noexcept
    {
			return SlotId != INVALID_SLOT_ID;
    }
	};

  struct SSignalArgs final
  {
    std::vector<std::any> Values;

    template<typename T>
    [[nodiscard]] const T& Get(size_t _index) const
    {
      return std::any_cast<const T&>(Values[_index]);
    }

    [[nodiscard]] size_t Count() const noexcept
    {
      return Values.size();
    }
  };
}