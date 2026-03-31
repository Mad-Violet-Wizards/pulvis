#pragma once

#include "assets/AssetPayload.hpp"

#include <cstdint>

namespace pulvis::fs::assets
{
	struct SAudioPayload : public TAssetPayload<EAssetType::Audio>
	{
		uint32_t SampleRate = 0;
		uint32_t ChannelCount = 0;
		uint32_t BitsPerSample = 0;
		uint64_t TotalSamples = 0;
	};
}