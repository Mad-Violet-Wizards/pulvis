#pragma once

#include "AssetPayload.hpp"
#include "FileTypes.hpp"
#include "FileBuffer.hpp"
#include "Assert.hpp"

#include <string>
#include <cstdint>
#include <memory>

namespace pulvis::fs::assets
{
//////////////////////////////////////////////////////////////////////////
	enum class EAssetState : uint8_t
	{
		Unknown,
		Registered,
		Loading,
		Loaded,
		Ready,
		Failed,
		Unloading,
		Unloaded
	};

//////////////////////////////////////////////////////////////////////////
	struct SAssetHandle
	{
		uint32_t Index = 0;
		uint32_t Generation = 0;

		bool IsValid() const { return Generation != 0; }
		bool operator==(const SAssetHandle& _other) const { return Index == _other.Index && Generation == _other.Generation; }
		bool operator!=(const SAssetHandle& _other) const { return !(*this == _other); }
	};

	constexpr SAssetHandle INVALID_ASSET_HANDLE{ 0, 0 };

//////////////////////////////////////////////////////////////////////////
	struct SAssetEntry
	{
		SAssetHandle Handle = INVALID_ASSET_HANDLE;
		EAssetType Type = EAssetType::Unknown;
		EAssetState State = EAssetState::Unknown;
		EDomain Domain = EDomain::Engine;
		std::string VirtualPath;
		CFileBuffer RawData;
		std::unique_ptr<IAssetPayload> Payload;
		uint64_t LastAccessTime = 0;
		uint32_t RefCount = 0;

		template<typename T>
		T* GetPayload()
		{
			static_assert(std::is_base_of<IAssetPayload, T>::value, "T must derive from IAssetPayload");
			ASSERT(Payload, "Payload is null.");
			ASSERT(Payload->GetType() == T::StaticType, "Payload type mismatch.");
			return static_cast<T*>(Payload.get());
		}

		template<typename T>
		const T* GetPayload() const
		{
			static_assert(std::is_base_of<IAssetPayload, T>::value, "T must derive from IAssetPayload");
			ASSERT(Payload, "Payload is null.");
			ASSERT(Payload->GetType() == T::StaticType, "Payload type mismatch.");
			return static_cast<const T*>(Payload.get());
		}
	};
}