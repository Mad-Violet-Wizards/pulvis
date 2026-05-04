#pragma once

#include "assets/AssetPayload.hpp"

#include <cstdint>

namespace pulvis::rendering
{
	struct SShaderPayload : public pulvis::fs::assets::TAssetPayload<pulvis::fs::EAssetType::Shader>
	{
		uint32_t ProgramID = 0;
	};
}