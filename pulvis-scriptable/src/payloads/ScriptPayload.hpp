#pragma once

#include "assets/AssetPayload.hpp"
#include "FileTypes.hpp"

#include <string>

namespace pulvis::scriptable
{
	struct SScriptPayload final : public pulvis::fs::assets::TAssetPayload<pulvis::fs::EAssetType::Script>
	{
		std::string Source;
	};
} // namespace pulvis::scriptable