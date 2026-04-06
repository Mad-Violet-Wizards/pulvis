#pragma once

#include "FileTypes.hpp"

namespace pulvis::fs::assets
{
	class IAssetPayload
	{
	public:

		virtual ~IAssetPayload() = default;
		virtual EAssetType GetType() const = 0;
	};

	template<EAssetType E>
	class TAssetPayload : public IAssetPayload
	{
	public:

		static constexpr EAssetType StaticType = E;
		virtual EAssetType GetType() const override { return E; }
	};
}