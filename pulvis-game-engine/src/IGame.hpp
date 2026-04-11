#pragma once

namespace pulvis::fs
{
    class CMountSystem;
    class CDomainRoots;
}

namespace pulvis::game_engine
{
    struct SEngineConfig;

    class IGame
    {
        public:

            virtual ~IGame() = default;

            /* Called before any system is allocated. */
            virtual void Configure(SEngineConfig& _config) = 0;

            /* 
                Called after DomainRoots and MountSystem are created and base domains 
                are bootstrapped (engine, game, user, dev).
                Override to mount additional game-specific directories 
                (e.g. levels, tilesets, mods).
            */
            virtual void OnMountFilesystems(pulvis::fs::CMountSystem& _mount_system, const pulvis::fs::CDomainRoots& _domain_roots) = 0;

            /* Called after all core systems are allocated (filesystem, rendering, level...). */
            virtual void OnInitialize() = 0;

            /* Called before engine systems are shut down. */
            virtual void OnShutdown() = 0;

            virtual void Frame(float _dt) = 0;
            virtual void Render() = 0;
    };
}