#pragma once

#include "IGame.hpp"
#include "EngineConfig.hpp"
#include "GameStateMachine.hpp"

#include <memory>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace pulvis::fs
{
	class CDomainRoots;
	class CMountSystem;

	namespace assets
	{
		class CAssetRegistry;
	}
}

namespace pulvis::rendering
{
	class CRenderService;
}

namespace pulvis::level
{
	class CLevelService;
}

namespace pulvis::threads
{
	class CMessageBus;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pulvis::game_engine
{
    class CGameBase : public IGame
    {
        public:

            CGameBase();
            virtual ~CGameBase();

            CGameBase(const CGameBase&) = delete;
            CGameBase& operator=(const CGameBase&) = delete;

            void Run();

        public:

            pulvis::fs::CDomainRoots& GetDomainRoots() const;
            pulvis::fs::CMountSystem& GetMountSystem() const;
            pulvis::fs::assets::CAssetRegistry& GetAssetRegistry() const;
            pulvis::rendering::CRenderService& GetRenderService() const;
            pulvis::level::CLevelService& GetLevelService() const;
            pulvis::threads::CMessageBus& GetMessageBus() const;

            CGameStateMachine& GetStateMachine();

        private:

            void Initialize();
            void InitializeFilesystem();
            void InitializeMessageBus();
            void InitializeServices();

            void Shutdown();
            void MainLoop();

        private:

            SEngineConfig m_Config;

            std::unique_ptr<pulvis::fs::CDomainRoots> m_DomainRoots;
            std::unique_ptr<pulvis::fs::CMountSystem> m_MountSystem;
            std::unique_ptr<pulvis::fs::assets::CAssetRegistry> m_AssetRegistry;
            std::unique_ptr<pulvis::rendering::CRenderService> m_RenderService;
            std::unique_ptr<pulvis::level::CLevelService> m_LevelService;
            std::unique_ptr<pulvis::threads::CMessageBus> m_MessageBus;

            CGameStateMachine m_StateMachine;

            using Clock = std::chrono::high_resolution_clock;
            Clock::time_point m_LastFrameTime;
    };
}