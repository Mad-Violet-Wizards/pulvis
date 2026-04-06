#pragma once

#include "assets/AssetPayload.hpp"

#include <cstdint>

namespace pulvis::fs::assets
{
	struct SShaderPayload : public TAssetPayload<EAssetType::Shader>
	{
		uint32_t ProgramID = 0;
	};
}