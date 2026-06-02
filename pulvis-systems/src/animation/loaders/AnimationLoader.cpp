#include "animation/loaders/AnimationLoader.hpp"
#include "animation/payloads/AnimationPayload.hpp"
#include "animation/AnimationRegistry.hpp"
#include "animation/AnimationEventRegistry.hpp"
#include "animation/AnimationDesc.hpp"
#include "animation/Animation.hpp"
#include "animation/serializers/AnimationSerializeTrait.hpp"
#include "payloads/TexturePayload.hpp"

#include "assets/AssetRegistry.hpp"
#include "serialization/SerializationArchive.hpp"
#include "serialization/JsonArchiveBackend.hpp"
#include "Logger.hpp"

#include <utility>

namespace pulvis::systems::animation
{
	using pulvis::fs::serialization::CSerializationArchive;
	using pulvis::fs::serialization::CJsonArchiveBackend;
	using pulvis::fs::serialization::EArchiveMode;

	namespace
	{
		EPlaybackMode ParsePlayback(const std::string& _s)
		{
			if (_s == "Single") { return EPlaybackMode::Single; }
			if (_s == "PingPong") { return EPlaybackMode::PingPong; }
			return EPlaybackMode::Looped;
		}
	} // namespace

	CAnimationLoader::CAnimationLoader(CAnimationRegistry& _registry,
		CAnimationEventRegistry& _event_registry,
		pulvis::fs::assets::CAssetRegistry& _asset_registry)
		: m_Registry(_registry)
		, m_EventRegistry(_event_registry)
		, m_AssetRegistry(_asset_registry)
	{
	}

	bool CAnimationLoader::Process(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("[AnimationLoader] '{}': raw data empty.", _entry.VirtualPath);
			return false;
		}

		CAnimationDesc desc;
		{
			CSerializationArchive archive(std::make_unique<CJsonArchiveBackend>(), EArchiveMode::Read);
			archive.SetInput(_entry.RawData);
			archive.Process<CAnimationDesc>("Animation", desc);
		}

		if (desc.Name.empty())
		{
			PULVIS_ERROR_LOG("[AnimationLoader] '{}': missing Animation.Name.", _entry.VirtualPath);
			return false;
		}

		auto anim = std::make_unique<SAnimation>();
		anim->Name = desc.Name;
		anim->Id = MakeAnimId(desc.Name);
		anim->Tileset.TexturePath = desc.TexturePath;

		if (!anim->Tileset.TexturePath.empty())
		{
			pulvis::fs::assets::SAssetHandle tex = m_AssetRegistry.Find(anim->Tileset.TexturePath);
			if (!tex.IsValid())
			{
				tex = m_AssetRegistry.RegisterAsset(_entry.Domain, anim->Tileset.TexturePath, pulvis::fs::EAssetType::Texture);
			}

			if (!m_AssetRegistry.LoadAndProcess(tex))
			{
				PULVIS_ERROR_LOG("[AnimationLoader] '{}': failed to load tileset texture '{}'.",
					_entry.VirtualPath, anim->Tileset.TexturePath);
				return false;
			}

			if (const pulvis::fs::assets::SAssetEntry* tex_entry = m_AssetRegistry.Get(tex))
			{
				if (tex_entry->Payload)
				{
					const auto* tp = static_cast<const pulvis::rendering::STexturePayload*>(tex_entry->Payload.get());
					anim->Tileset.TextureID = tp->GPUHandle;
					anim->Tileset.TextureSize = { static_cast<float>(tp->Width), static_cast<float>(tp->Height) };
				}
			}
		}

		anim->Sequences.reserve(desc.Sequences.size());
		for (uint32_t i = 0; i < desc.Sequences.size(); ++i)
		{
			const CAnimationSequenceDesc& sd = desc.Sequences[i];
			if (sd.Name.empty())
			{
				PULVIS_ERROR_LOG("[AnimationLoader] '{}': sequence #{} empty name.", _entry.VirtualPath, i);
				return false;
			}

			SAnimationSequence seq;
			seq.Name = sd.Name;
			seq.Id = MakeSequenceId(sd.Name);
			seq.Playback = ParsePlayback(sd.Playback);
			seq.Frames = sd.Frames;

			for (const CAnimationFrame& f : seq.Frames) 
			{ 
				seq.TotalDurationMs += f.Duration; 
			}

			seq.Events.reserve(sd.Events.size());
			for (const CAnimationSequenceEventDesc& ed : sd.Events)
			{
				const event_id_t id = m_EventRegistry.FindId(ed.AnimEventName);
				if (id == INVALID_EVENT)
				{
					PULVIS_ERROR_LOG("[AnimationLoader] '{}': unknown event '{}' in '{}'.", _entry.VirtualPath, ed.AnimEventName, sd.Name);
					return false;
				}
				if (ed.Frame >= seq.Frames.size())
				{
					PULVIS_ERROR_LOG("[AnimationLoader] '{}': event '{}' frame {} out of range in '{}' ({} frames).",
						_entry.VirtualPath, ed.AnimEventName, ed.Frame, sd.Name, seq.Frames.size());
					return false;
				}
				seq.Events.push_back({ id, ed.Frame });
			}

			if (!anim->SequenceLookup.try_emplace(seq.Id, static_cast<uint32_t>(anim->Sequences.size())).second)
			{
				PULVIS_ERROR_LOG("[AnimationLoader] '{}': duplicate sequence '{}'.", _entry.VirtualPath, sd.Name);
				return false;
			}
			anim->Sequences.push_back(std::move(seq));
		}

		const anim_id_t id = m_Registry.Register(std::move(anim));
		if (id == INVALID_ANIM)
		{
			PULVIS_ERROR_LOG("[AnimationLoader] '{}': registry rejected animation.", _entry.VirtualPath);
			return false;
		}

		auto payload = std::make_unique<SAnimationPayload>();
		payload->AnimationId = id;
		_entry.Payload = std::move(payload);
		_entry.RawData.Clear();
		return true;
	}

	bool CAnimationLoader::Unload(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (auto* p = dynamic_cast<SAnimationPayload*>(_entry.Payload.get()))
		{
			m_Registry.Unregister(p->AnimationId);
		}
		_entry.Payload.reset();
		return true;
	}
} // namespace pulvis::systems::animation