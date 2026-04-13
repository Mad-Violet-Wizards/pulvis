#pragma once

namespace pulvis::game_engine
{
    struct SEngineConfig;

    class IGame
    {
        public:

            virtual ~IGame() = default;

            /* Called before any system is allocated. */
            virtual void Configure(SEngineConfig& _config) = 0;

            virtual void InitializeGameFilesystems() = 0;
            virtual void InitializeGameServices() = 0;

            /* Called after all core systems are allocated (filesystem, rendering, level...). */
            virtual void OnInitialize() = 0;

            /* Called before engine systems are shut down. */
            virtual void OnShutdown() = 0;

            virtual void Frame(float _dt) = 0;
            virtual void Render() = 0;
    };
}