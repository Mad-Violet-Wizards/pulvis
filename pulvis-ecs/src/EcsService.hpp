#pragma once

#include "EcsWorld.hpp"
#include "SystemManager.hpp"
#include "TemplateCache.hpp"
#include "EcsScriptableNode.hpp"
#include "TemplateScriptableNode.hpp"
#include "sigslot/SignalCache.hpp"
#include "sigslot/SignalScriptBridge.hpp"

#include <memory>

namespace pulvis::scriptable { class CScriptableService; }
namespace pulvis::threads { class CJobSystem; }
namespace pulvis::events { class CEventDispatcher; }
namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::rendering { class CRenderQueue; class CRenderLayerCache; }

namespace pulvis::ecs
{
	class CEcsService final
	{
	public:

		CEcsService(pulvis::scriptable::CScriptableService& _scriptable,
			pulvis::fs::assets::CAssetRegistry& _asset_registry,
			pulvis::events::CEventDispatcher& _event_dispatcher,
			pulvis::threads::CJobSystem* _job_system = nullptr);
		~CEcsService();

		CEcsService(const CEcsService&) = delete;
		CEcsService& operator=(const CEcsService&) = delete;

		void Initialize(pulvis::rendering::CRenderQueue& _render_queue, pulvis::rendering::CRenderLayerCache& _render_layer_cache);
		void Shutdown();
		void Frame(float _delta_time);

		void LoadAndExecuteScripts();

		[[nodiscard]] CWorld& GetWorld() { return *m_World; }
		[[nodiscard]] const CWorld& GetWorld() const { return *m_World; }
		[[nodiscard]] CSystemManager& GetSystemManager() { return *m_SystemManager; }
		[[nodiscard]] const CSystemManager& GetSystemManager() const { return *m_SystemManager; }
		[[nodiscard]] CTemplateCache& GetTemplateCache() { return *m_TemplateCache; }
		[[nodiscard]] const CTemplateCache& GetTemplateCache() const { return *m_TemplateCache; }
		[[nodiscard]] CSignalCache& GetSignalCache() { return *m_SignalCache; }
		[[nodiscard]] const CSignalCache& GetSignalCache() const { return *m_SignalCache; }
		[[nodiscard]] CSignalScriptBridge& GetSignalBridge() { return *m_SignalBridge; }
		[[nodiscard]] const CSignalScriptBridge& GetSignalBridge() const { return *m_SignalBridge; }

	private:

		void RegisterBuiltinComponents();
		void RegisterBuiltinSystems(pulvis::rendering::CRenderQueue& _render_queue, pulvis::rendering::CRenderLayerCache& _render_layer_cache);

	private:

		pulvis::scriptable::CScriptableService& m_Scriptable;
		pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
		pulvis::events::CEventDispatcher& m_EventDispatcher;
		pulvis::threads::CJobSystem* m_JobSystem = nullptr;

		std::unique_ptr<CWorld>                  m_World;
		std::unique_ptr<CSignalCache>            m_SignalCache;
		std::unique_ptr<CSignalScriptBridge>     m_SignalBridge;
		std::unique_ptr<CTemplateCache>          m_TemplateCache;
		std::unique_ptr<CSystemManager>          m_SystemManager;
		std::shared_ptr<CEcsScriptableNode>      m_EcsNode;
		std::shared_ptr<CTemplateScriptableNode> m_TemplateNode;

		bool m_Initialized = false;
	};
} // namespace pulvis::ecs