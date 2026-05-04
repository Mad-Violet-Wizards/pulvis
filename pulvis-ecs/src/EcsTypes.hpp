#pragma once

#include <cstdint>
#include <limits>

namespace pulvis::ecs
{
	using entity_index_t = uint32_t;
	using entity_generation_t = uint32_t;
	using component_id_t = uint32_t;
	using template_id_t = uint32_t;
	using preset_id_t = uint32_t;
	using hierarchy_layer_t = uint8_t;

	static constexpr entity_index_t INVALID_ENTITY_INDEX = std::numeric_limits<entity_index_t>::max();
	static constexpr entity_generation_t INVALID_ENTITY_GENERATION = 0;
	static constexpr component_id_t INVALID_COMPONENT_ID = std::numeric_limits<component_id_t>::max();
	static constexpr template_id_t INVALID_TEMPLATE_ID = std::numeric_limits<template_id_t>::max();
	static constexpr preset_id_t INVALID_PRESET_ID = std::numeric_limits<preset_id_t>::max();

	static constexpr size_t COMPONENT_PAGE_SIZE = 4096;
	static constexpr size_t MAX_COMPONENT_TYPES = 256;
}