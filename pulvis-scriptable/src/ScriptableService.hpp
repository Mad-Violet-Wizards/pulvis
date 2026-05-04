#pragma once

#include "ScriptHost.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::scriptable
{
	struct SScriptPayload;
	class IScriptableNode;

	/*
		CScriptableService - DI entry point for everything Lua-related.

		Owns the single CScriptHost (= the single sol::state) and the
		single CScriptLoader registered against EAssetType::Script in
		the asset registry. Other services (CLevelService, CEcsService,
		future input / player / dialogue / AI services) take a reference
		to this service, register their IScriptableNode to expose their
		Lua keywords, then drive their own load + execute order via the
		asset registry and ExecuteScriptByPath().
	*/
	class CScriptableService
	{
	public:

		explicit CScriptableService(pulvis::fs::assets::CAssetRegistry& _asset_registry);
		~CScriptableService();

		CScriptableService(const CScriptableService&) = delete;
		CScriptableService& operator=(const CScriptableService&) = delete;

		void Initialize();
		void Shutdown();

		[[nodiscard]] bool IsInitialized() const { return m_Initialized; }

		void RegisterScriptableNode(std::shared_ptr<IScriptableNode> _scriptable_node);

		[[nodiscard]] const SScriptPayload* FindScript(std::string_view _virtual_path) const;
		bool ExecuteScript(const SScriptPayload& _payload, std::string_view _chunk_name);
		bool ExecuteScriptByPath(std::string_view _virtual_path);

		[[nodiscard]] std::string_view GetCurrentlyExecutingScript() const { return m_CurrentlyExecuting; }

		CScriptHost& GetHost() { return m_Host; }
		const CScriptHost& GetHost() const { return m_Host; }

		sol::state& GetLuaState() { return m_Host.Raw(); }
		const sol::state& GetLuaState() const { return m_Host.Raw(); }

	private:

		pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
		std::vector<std::shared_ptr<IScriptableNode>> m_ScriptableNodes;
		std::string                                   m_CurrentlyExecuting;
		CScriptHost                                   m_Host;
		bool                                          m_Initialized = false;
	};
} // namespace pulvis::scriptable