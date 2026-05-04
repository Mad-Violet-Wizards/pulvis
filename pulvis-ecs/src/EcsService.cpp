#include "EcsService.hpp"

#include "components/LogicComponent.hpp"
#include "components/HierarchyComponent.hpp"
#include "components/TagComponent.hpp"
#include "components/TransformComponent.hpp"
#include "components/SpriteComponent.hpp"

#include "systems/LogicSystem.hpp"
#include "systems/HierarchySystem.hpp"
#include "systems/TransformSystem.hpp"
#include "systems/SpriteRenderSystem.hpp"

#include "ScriptableService.hpp"
#include "assets/AssetRegistry.hpp"
#include "assets/AssetEntry.hpp"
#include "RenderLayerCache.hpp"
#include "RenderQueue.hpp"
#include "FileTypes.hpp"
#include "Logger.hpp"

namespace pulvis::ecs
{
	namespace
	{
		constexpr const char* TEMPLATES_DIR = "game/scripts/templates";
		constexpr const char* PRESETS_DIR = "game/scripts/presets";
	}

	CEcsService::CEcsService(pulvis::scriptable::CScriptableService& _scriptable,
		pulvis::fs::assets::CAssetRegistry& _asset_registry,
		pulvis::events::CEventDispatcher& _event_dispatcher,
		pulvis::threads::CJobSystem* _job_system)
		: m_Scriptable(_scriptable)
		, m_AssetRegistry(_asset_registry)
		, m_EventDispatcher(_event_dispatcher)
		, m_JobSystem(_job_system)
	{
	}

	CEcsService::~CEcsService()
	{
		if (m_Initialized) { Shutdown(); }
	}

	void CEcsService::Initialize(pulvis::rendering::CRenderQueue& _render_queue, pulvis::rendering::CRenderLayerCache& _render_layer_cache)
	{
		if (m_Initialized) { return; }

		m_World = std::make_unique<CWorld>();
		m_SignalCache = std::make_unique<CSignalCache>();
		m_SignalBridge = std::make_unique<CSignalScriptBridge>(*m_SignalCache);
		m_SignalBridge->RegisterUserTypes(m_Scriptable.GetLuaState());

		m_World->SetSignalRegistry(m_SignalCache.get());

		m_TemplateCache = std::make_unique<CTemplateCache>();
		m_TemplateCache->SetSignalSubsystem(m_SignalCache.get(), m_SignalBridge.get());

		m_SystemManager = std::make_unique<CSystemManager>();
		m_SystemManager->SetJobSystem(m_JobSystem);

		RegisterBuiltinComponents();
		RegisterBuiltinSystems(_render_queue, _render_layer_cache);

		m_EcsNode = std::make_shared<CEcsScriptableNode>(*m_World, *m_TemplateCache);
		m_Scriptable.RegisterScriptableNode(m_EcsNode);

		m_TemplateNode = std::make_shared<CTemplateScriptableNode>(
			*m_World, *m_TemplateCache, *m_SignalCache, m_EventDispatcher);
		m_Scriptable.RegisterScriptableNode(m_TemplateNode);

		m_Initialized = true;
		PULVIS_INFO_LOG("EcsService initialized.");
	}

	void CEcsService::Shutdown()
	{
		if (!m_Initialized) { return; }
		m_TemplateNode.reset();
		m_EcsNode.reset();
		m_SystemManager.reset();
		m_TemplateCache.reset();
		if (m_World) { m_World->SetSignalRegistry(nullptr); }
		m_SignalBridge.reset();
		m_SignalCache.reset();
		m_World.reset();
		m_Initialized = false;
		PULVIS_INFO_LOG("EcsService shut down.");
	}

	void CEcsService::Frame(float _delta_time)
	{
		if (!m_Initialized) { return; }
		m_SystemManager->Frame(*m_World, _delta_time);
	}

	void CEcsService::LoadAndExecuteScripts()
	{
		const std::pair<pulvis::fs::EDomain, const char*> stages[] =
		{
			{ pulvis::fs::EDomain::Game, TEMPLATES_DIR },
			{ pulvis::fs::EDomain::Game, PRESETS_DIR   },
		};

		for (const auto& [domain, dir] : stages)
		{
			m_AssetRegistry.LoadDirectory(domain, dir, true);

			const std::vector<pulvis::fs::assets::SAssetHandle> handles =
				m_AssetRegistry.EnumerateByPrefix(pulvis::fs::EAssetType::Script, dir);

			uint32_t executed = 0;
			for (const pulvis::fs::assets::SAssetHandle& handle : handles)
			{
				const pulvis::fs::assets::SAssetEntry* entry = m_AssetRegistry.Get(handle);
				if (entry == nullptr) { continue; }
				if (m_Scriptable.ExecuteScriptByPath(entry->VirtualPath)) { ++executed; }
			}

			PULVIS_INFO_LOG("EcsService: executed {} script(s) under '{}'.", executed, dir);
		}
	}

	void CEcsService::RegisterBuiltinComponents()
	{
		auto register_component = [&]<typename T>(std::string _name)
		{
			SComponentTraits traits;
			traits.TypeName = std::move(_name);
			m_World->RegisterComponent<T>(std::move(traits));
		};

		register_component.template operator() <SLogicComponent> ("SLogicComponent");
		register_component.template operator() <SHierarchyComponent> ("SHierarchyComponent");
		register_component.template operator() <STagComponent> ("STagComponent");
		register_component.template operator() <STransformComponent> ("STransformComponent");
		register_component.template operator() < SSpriteComponent > ("SSpriteComponent");
	}

	void CEcsService::RegisterBuiltinSystems(pulvis::rendering::CRenderQueue& _render_queue, pulvis::rendering::CRenderLayerCache& _render_layer_cache)
	{
		m_SystemManager->Register(std::make_unique<CLogicSystem>(m_Scriptable, *m_SignalBridge));
		m_SystemManager->Register(std::make_unique<CHierarchySystem>());
		m_SystemManager->Register(std::make_unique<CTransformSystem>());
		m_SystemManager->Register(std::make_unique<CSpriteRenderSystem>(_render_queue, _render_layer_cache));
	}
} // namespace pulvis::ecs