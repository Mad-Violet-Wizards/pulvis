#include "LevelService.hpp"

#include "MessageBus.hpp"
#include "MountSystem.hpp"
#include "FilePath.hpp"
#include "FileBuffer.hpp"
#include "RenderLayerCache.hpp"
#include "Logger.hpp"

#include "ScriptableService.hpp"
#include "ScriptHost.hpp"
#include "TableView.hpp"

#include <format>

namespace pulvis::level
{
	namespace
	{
		std::string MakeLayerScriptPath(const std::string& _levelPath)
		{
			return std::format("{}/layers.lua", _levelPath);
		}
	}

	CLevelService::CLevelService(pulvis::fs::assets::CAssetRegistry& _asset_registry,
		pulvis::fs::CMountSystem& _mount_system,
		pulvis::scriptable::CScriptableService& _scriptable_service)
		: m_AssetRegistry(_asset_registry)
		, m_MountSystem(_mount_system)
		, m_ScriptableService(_scriptable_service)
	{
	}

	void CLevelService::Initialize(pulvis::threads::CMessageBus& _messageBus, uint32_t _ioChannelId, uint32_t _mainChannelId)
	{
		m_ChunkIO = std::make_unique<CChunkIO>(m_MountSystem, _messageBus, _ioChannelId, _mainChannelId);
		m_ChunkIO->RegisterHandlers();

		PULVIS_INFO_LOG("LevelService initialized.");
	}

	void CLevelService::Shutdown()
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			const uint32_t saved = SaveDirtyChunks();
			if (saved > 0)
			{
				PULVIS_INFO_LOG("LevelService::Shutdown - Saved {} dirty chunks.", saved);
			}
		}

		m_ActiveLevel.reset();
		m_ChunkIO.reset();
		m_ActiveLevelPath.clear();

		PULVIS_INFO_LOG("LevelService shut down.");
	}

	void CLevelService::CreateEmptyLevel(const std::string& _levelPath)
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			SaveDirtyChunks();
		}

		m_ActiveLevel = std::make_unique<CLevel>();
		m_ActiveLevelPath = _levelPath;

		PULVIS_INFO_LOG("Empty level created: {}", _levelPath);
	}

	void CLevelService::SetActiveLevel(std::unique_ptr<CLevel> _level, const std::string& _levelPath)
	{
		if (m_ActiveLevel && m_ChunkIO)
		{
			SaveDirtyChunks();
		}

		m_ActiveLevel = std::move(_level);
		m_ActiveLevelPath = _levelPath;
	}

	CLevel* CLevelService::GetActiveLevel() { return m_ActiveLevel.get(); }
	const CLevel* CLevelService::GetActiveLevel() const { return m_ActiveLevel.get(); }

	bool CLevelService::LoadChunkSync(const SChunkCoord& _coord)
	{
		if (!m_ActiveLevel || !m_ChunkIO) { return false; }
		if (m_ActiveLevel->HasChunk(_coord)) { return true; }

		SChunk* chunk = m_ActiveLevel->GetOrCreateChunk(_coord);
		return m_ChunkIO->LoadChunkSync(m_ActiveLevelPath, _coord, *chunk);
	}

	bool CLevelService::SaveChunkSync(const SChunk& _chunk)
	{
		if (!m_ChunkIO) { return false; }
		return m_ChunkIO->SaveChunkSync(m_ActiveLevelPath, _chunk);
	}

	uint32_t CLevelService::LoadChunkBatchSync(const std::vector<SChunkCoord>& _coords)
	{
		if (!m_ActiveLevel || !m_ChunkIO) { return 0; }
		return m_ChunkIO->LoadChunkBatchSync(m_ActiveLevelPath, _coords, *m_ActiveLevel);
	}

	uint32_t CLevelService::SaveDirtyChunks()
	{
		if (!m_ActiveLevel || !m_ChunkIO) { return 0; }
		return m_ChunkIO->SaveDirtyChunksSync(m_ActiveLevelPath, *m_ActiveLevel);
	}

	bool CLevelService::ChunkExistsOnDisk(const std::string& levelPath, const SChunkCoord& _coord) const
	{
		if (!m_ChunkIO) { return false; }
		return m_ChunkIO->ChunkExistsOnDisk(levelPath, _coord);
	}

	void CLevelService::RequestLoadChunk(const SChunkCoord& _coord)
	{
		if (!m_ChunkIO) { return; }
		m_ChunkIO->RequestLoadChunk(m_ActiveLevelPath, _coord);
	}

	void CLevelService::RequestLoadChunks(const std::vector<SChunkCoord>& _coords)
	{
		if (!m_ChunkIO) { return; }
		m_ChunkIO->RequestLoadChunks(m_ActiveLevelPath, _coords);
	}

	uint32_t CLevelService::ApplyPendingChunks()
	{
		if (!m_ActiveLevel || !m_ChunkIO) { return 0; }
		return m_ChunkIO->ApplyPendingChunks(*m_ActiveLevel);
	}

	size_t CLevelService::PendingChunkCount() const
	{
		if (!m_ChunkIO) { return 0; }
		return m_ChunkIO->PendingCount();
	}

	bool CLevelService::LoadLayerScript(pulvis::rendering::CRenderLayerCache& _layerCache)
	{
		if (!m_ActiveLevel)
		{
			PULVIS_WARNING_LOG("LoadLayerScript - no active level.");
			return false;
		}

		const std::string script_path = MakeLayerScriptPath(m_ActiveLevelPath);
		const pulvis::fs::CFilePath file_path(script_path);

		auto resolved = m_MountSystem.Resolve(pulvis::fs::EDomain::Game, file_path, false);
		if (!resolved)
		{
			PULVIS_WARNING_LOG("LoadLayerScript - script not found: {}", script_path);
			return false;
		}

		pulvis::fs::CFileBuffer buffer;
		if (resolved.Source->Read(resolved.LocalPath, buffer) != pulvis::fs::EFileResult::Success)
		{
			PULVIS_ERROR_LOG("LoadLayerScript - failed to read: {}", script_path);
			return false;
		}

		// Run the script inside an isolated environment to avoid global pollution.
		// BuildLayers is fetched from THIS environment, never reaches sol::state globals.
		pulvis::scriptable::CScriptHost& host = m_ScriptableService.GetHost();
		sol::environment env = host.MakeEnvironment();

		const std::string_view source(reinterpret_cast<const char*>(buffer.Data()), buffer.Size());

		if (!host.RunStringInEnv(env, source, script_path))
		{
			return false;
		}

		sol::optional<sol::table> layers_opt = host.CallInEnv<sol::table>(env, "BuildLayers");
		if (!layers_opt.has_value())
		{
			PULVIS_ERROR_LOG("LoadLayerScript - BuildLayers() missing or failed in {}.", script_path);
			return false;
		}

		m_ActiveLevel->ClearLayers();

		pulvis::scriptable::CTableView layers_view(layers_opt.value());
		layers_view.ForEachArrayItem([&](size_t /*_index*/, sol::object _value)
			{
				if (!_value.is<sol::table>()) { return; }

				pulvis::scriptable::CTableView desc_view(_value.as<sol::table>());

				SLevelLayerDesc desc;
				desc.Name = desc_view.ReadOr<std::string>("Name", "");
				desc.RenderLayerName = desc_view.ReadOr<std::string>("RenderLayerName", desc.Name);
				desc.SortKey = desc_view.ReadOr<int32_t>("SortKey", 0);
				desc.Visible = desc_view.ReadOr<bool>("Visible", true);
				desc.CollisionSource = desc_view.ReadOr<bool>("CollisionSource", true);
				desc.DefaultMaterializeOnEdit = desc_view.ReadOr<bool>("DefaultMaterializeOnEdit", false);

				pulvis::rendering::SRenderLayerHandle handle = _layerCache.Find(desc.RenderLayerName);
				if (!handle.IsValid())
				{
					pulvis::rendering::SRenderLayerDesc rl_desc{};
					rl_desc.DebugName = desc.RenderLayerName;
					rl_desc.SortKey = desc.SortKey;
					rl_desc.Blend = pulvis::rendering::ERenderLayerBlend::AlphaBlend;
					handle = _layerCache.Register(rl_desc);
				}
				desc.Render = handle;

				m_ActiveLevel->AddLayer(std::move(desc));
			});

		PULVIS_INFO_LOG("LoadLayerScript - loaded {} layers from {}.",
			m_ActiveLevel->GetLayerCount(), script_path);
		return true;
	}

	bool CLevelService::SaveLayerScript() const
	{
		if (!m_ActiveLevel)
		{
			return false;
		}

		const std::string script_path = MakeLayerScriptPath(m_ActiveLevelPath);
		const pulvis::fs::CFilePath file_path(script_path);

		auto resolved = m_MountSystem.Resolve(pulvis::fs::EDomain::Game, file_path, true);
		if (!resolved)
		{
			PULVIS_ERROR_LOG("SaveLayerScript - no writable mount for: {}", script_path);
			return false;
		}

		std::string out;
		out += "-- Auto-generated by CLevelService::SaveLayerScript.\n";
		out += "-- Edited by the level designer; defines logical layers of the level.\n\n";
		out += "function BuildLayers()\n";
		out += "    return {\n";

		for (const SLevelLayerDesc& layer : m_ActiveLevel->GetLayers())
		{
			out += "        {\n";
			out += std::format("            Name                     = \"{}\",\n", layer.Name);
			out += std::format("            RenderLayerName          = \"{}\",\n", layer.RenderLayerName);
			out += std::format("            SortKey                  = {},\n", layer.SortKey);
			out += std::format("            Visible                  = {},\n", layer.Visible ? "true" : "false");
			out += std::format("            CollisionSource          = {},\n", layer.CollisionSource ? "true" : "false");
			out += std::format("            DefaultMaterializeOnEdit = {},\n", layer.DefaultMaterializeOnEdit ? "true" : "false");
			out += "        },\n";
		}

		out += "    }\n";
		out += "end\n";

		pulvis::fs::CFileBuffer buffer;
		buffer.Resize(out.size());
		std::memcpy(buffer.Data(), out.data(), out.size());

		if (resolved.Source->Write(resolved.LocalPath, buffer) != pulvis::fs::EFileResult::Success)
		{
			PULVIS_ERROR_LOG("SaveLayerScript - failed to write: {}", script_path);
			return false;
		}

		PULVIS_INFO_LOG("SaveLayerScript - wrote {} layers to {}.",
			m_ActiveLevel->GetLayerCount(), script_path);
		return true;
	}
}