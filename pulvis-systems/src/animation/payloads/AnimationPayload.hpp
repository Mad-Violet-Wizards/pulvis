#pragma once

#include "assets/AssetPayload.hpp"
#include "FileTypes.hpp"
#include "animation/AnimationTypes.hpp"

namespace pulvis::systems::animation
{
	struct SAnimationPayload final : public pulvis::fs::assets::TAssetPayload<pulvis::fs::EAssetType::Animation>
	{
		anim_id_t AnimationId = INVALID_ANIM;
	};
} // namespace pulvis::systems::animation