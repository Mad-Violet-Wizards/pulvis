#include "animation/loaders/AnimationEventLoader.hpp"
#include "animation/AnimationEventRegistry.hpp"
#include "animation/AnimationEvent.hpp"
#include "animation/serializers/AnimationSerializeTrait.hpp"

#include "assets/AssetEntry.hpp"
#include "serialization/SerializationArchive.hpp"
#include "serialization/JsonArchiveBackend.hpp"
#include "Logger.hpp"

#include <vector>

namespace pulvis::systems::animation
{
	using pulvis::fs::serialization::CSerializationArchive;
	using pulvis::fs::serialization::CJsonArchiveBackend;
	using pulvis::fs::serialization::EArchiveMode;

	CAnimationEventsLoader::CAnimationEventsLoader(CAnimationEventRegistry& _registry)
		: m_Registry(_registry)
	{
	}

	bool CAnimationEventsLoader::Process(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("[AnimationEventsLoader] '{}': raw data empty.", _entry.VirtualPath);
			return false;
		}

		CSerializationArchive archive(std::make_unique<CJsonArchiveBackend>(), EArchiveMode::Read);
		archive.SetInput(_entry.RawData);

		std::vector<SAnimationEvent> events;
		archive.Process<std::vector<SAnimationEvent>>("AnimEvents", events);

		uint32_t accepted = 0;
		for (SAnimationEvent& ev : events)
		{
			if (m_Registry.Register(std::move(ev))) { ++accepted; }
		}

		PULVIS_INFO_LOG("[AnimationEventsLoader] '{}': registered {}/{} events.", _entry.VirtualPath, accepted, events.size());
		_entry.RawData.Clear();
		return accepted > 0;
	}

	bool CAnimationEventsLoader::Unload(pulvis::fs::assets::SAssetEntry& _entry)
	{
		_entry.Payload.reset();
		return true;
	}
} // namespace pulvis::systems::animation