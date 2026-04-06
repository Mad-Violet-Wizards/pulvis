#pragma once

#include <cstdint>

namespace pulvis::rtti
{
	using type_id_t = uint32_t;
	constexpr static type_id_t INVALID_TYPE_ID = UINT32_MAX;
}
