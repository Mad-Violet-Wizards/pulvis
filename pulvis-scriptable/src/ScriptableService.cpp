#include "ScriptableService.hpp"

#include "payloads/ScriptPayload.hpp"
#include "loaders/ScriptLoader.hpp"
#include "ScriptableNode.hpp"

#include "assets/AssetRegistry.hpp"
#include "Logger.hpp"

#include <utility>

namespace pulvis::scriptable
{
	CScriptableService::CScriptableService(pulvis::fs::assets::CAssetRegistry& _asset_registry)
		: m_AssetRegistry(_asset_registry)
	{
	}

	CScriptableService::~CScriptableService() = default;

	void CScriptableService::Initialize()
	{
		if (m_Initialized) { return; }

		sol::state& lua = m_Host.Raw();
		for (auto& node : m_ScriptableNodes)
		{
			if (node) { node->RegisterLuaBindings(lua); }
		}

		m_Initialized = true;
		PULVIS_INFO_LOG("ScriptableService initialized ({} pre-registered nodes).",
			m_ScriptableNodes.size());
	}

	void CScriptableService::Shutdown()
	{
		if (!m_Initialized) { return; }
		m_ScriptableNodes.clear();
		m_CurrentlyExecuting.clear();
		m_Initialized = false;
		PULVIS_INFO_LOG("ScriptableService shut down.");
	}

	void CScriptableService::RegisterScriptableNode(std::shared_ptr<IScriptableNode> _scriptable_node)
	{
		if (!_scriptable_node) { return; }

		if (m_Initialized)
		{
			_scriptable_node->RegisterLuaBindings(m_Host.Raw());
		}

		m_ScriptableNodes.push_back(std::move(_scriptable_node));
	}

	const SScriptPayload* CScriptableService::FindScript(std::string_view _virtual_path) const
	{
		const pulvis::fs::assets::SAssetHandle handle = m_AssetRegistry.Find(std::string(_virtual_path));
		if (!handle.IsValid()) { return nullptr; }

		const pulvis::fs::assets::SAssetEntry* entry = m_AssetRegistry.Get(handle);
		if (entry == nullptr || entry->Type != pulvis::fs::EAssetType::Script) { return nullptr; }
		if (entry->Payload == nullptr) { return nullptr; }

		return entry->GetPayload<SScriptPayload>();
	}

	bool CScriptableService::ExecuteScript(const SScriptPayload& _payload, std::string_view _chunk_name)
	{
		std::string previous = std::move(m_CurrentlyExecuting);
		m_CurrentlyExecuting.assign(_chunk_name);

		const bool ok = m_Host.RunString(_payload.Source, _chunk_name);

		m_CurrentlyExecuting = std::move(previous);
		return ok;
	}

	bool CScriptableService::ExecuteScriptByPath(std::string_view _virtual_path)
	{
		const auto* payload = FindScript(_virtual_path);
		if (payload == nullptr)
		{
			PULVIS_WARNING_LOG("ExecuteScriptByPath: script not found '{}'.", _virtual_path);
			return false;
		}
		return ExecuteScript(*payload, _virtual_path);
	}
} // namespace pulvis::scriptable