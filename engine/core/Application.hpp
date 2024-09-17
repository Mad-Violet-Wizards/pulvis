
#pragma once

#include "engine/rendering/Window.hpp"
#include "engine/filesystem/Filesystem.hpp"
#include "engine/time/Timer.hpp"

namespace engine
{
	namespace core
	{
		struct ApplicationSetup;

		class PULVIS_API Application
		{
			public:

				Application(const ApplicationSetup& _app_setup);
				~Application();
				
				bool IsCloseRequested() const;
				void Run();

				void PreFrame();
				void Frame();
				void PostFrame();

				const engine::fs::Filesystem& GetEngineFs() const;

			private:

				void InitializeCoreSystems(const ApplicationSetup& _app_setup);
				void InitializeWindow(const ApplicationSetup& _app_setup);
				void UpdateFrameTime();

			private:
				
				engine::time::Timer m_FrameTimer;
				float m_FrameTime;
				bool m_bCoreSystemsInitialized;
				
				engine::rendering::CWindow* m_Window;

				engine::fs::Filesystem m_EngineFilesystem;
		};
	}
}
