#include "assets/loaders/AnimationLoader.hpp"
#include "assets/payloads/AnimationPayload.hpp"
#include "Logger.hpp"

#include <nlohmann/json.hpp>

namespace pulvis::fs::assets
{
	CAnimationLoader::CAnimationLoader(CAssetRegistry& _registry)
		: m_Registry(_registry)
	{
	}

	bool CAnimationLoader::Process(SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("Cannot process animation asset '{}': Raw data is empty.", _entry.VirtualPath);
			return false;
		}

		nlohmann::json root;

		try
		{
			root = nlohmann::json::parse(
				_entry.RawData.Data(),
				_entry.RawData.Data() + _entry.RawData.Size()
			);
		}
		catch (const nlohmann::json::exception& e)
		{
			PULVIS_ERROR_LOG("JSON parse failed: {} ({})", _entry.VirtualPath, e.what());
			return false;
		}

		if (!root.contains("frames") || !root.contains("meta"))
		{
			PULVIS_ERROR_LOG("Missing frames/meta: {}", _entry.VirtualPath);
			return false;
		}

		const auto& meta = root["meta"];
		const std::string texture_path = meta.value("image", "");

		if (texture_path.empty())
		{
			PULVIS_ERROR_LOG("Missing meta.image: {}", _entry.VirtualPath);
			return false;
		}

		SAssetHandle tex_handle = m_Registry.Find(texture_path);
		if (!tex_handle.IsValid())
		{
			tex_handle = m_Registry.RegisterAsset(_entry.Domain, texture_path, EAssetType::Texture);
		}

		const float atlas_w = static_cast<float>(meta["size"].value("w", 1));
		const float atlas_h = static_cast<float>(meta["size"].value("h", 1));

		const auto& frames_json = root["frames"];

		std::vector<SAnimationFrame> frames;
		frames.reserve(frames_json.size());

		uint32_t frame_w = 0;
		uint32_t frame_h = 0;

		for (const auto& f : frames_json)
		{
			const auto& rect = f["frame"];

			SAnimationFrame frame;
			frame.UV_X = static_cast<float>(rect.value("x", 0)) / atlas_w;
			frame.UV_Y = static_cast<float>(rect.value("y", 0)) / atlas_h;
			frame.UV_W = static_cast<float>(rect.value("w", 0)) / atlas_w;
			frame.UV_H = static_cast<float>(rect.value("h", 0)) / atlas_h;
			frame.DurationMs = f.value("duration", 100u);

			if (frame_w == 0)
			{
				frame_w = rect.value("w", 0u);
				frame_h = rect.value("h", 0u);
			}

			frames.push_back(frame);
		}

		std::vector<SAnimationTag> tags;

		if (meta.contains("frameTags"))
		{
			for (const auto& t : meta["frameTags"])
			{
				SAnimationTag tag;
				tag.Name = t.value("name", "");
				tag.FromFrame = t.value("from", 0u);
				tag.ToFrame = t.value("to", 0u);
				tags.push_back(std::move(tag));
			}
		}

		auto payload = std::make_unique<SAnimationPayload>();
		payload->TextureHandle = tex_handle;
		payload->FrameWidth = frame_w;
		payload->FrameHeight = frame_h;
		payload->Frames = std::move(frames);
		payload->Tags = std::move(tags);

		_entry.Payload = std::move(payload);
		_entry.RawData.Clear();

		return true;
	}

	bool CAnimationLoader::Unload(SAssetEntry& _entry)
	{
		_entry.Payload = nullptr;
		return true;
	}
}