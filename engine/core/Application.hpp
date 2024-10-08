
#pragma once

#include "engine/rendering/Window.hpp"
#include "engine/filesystem/Filesystem.hpp"
#include "engine/time/Timer.hpp"
#include "Setup.hpp"
#include "Export.hpp"

namespace engine
{
	namespace core
	{
		//////////////////////////////////////////////////////////////////////////
		struct PULVIS_API SApplicationContext
		{
			SApplicationSetup m_AppSetup;
		};

		static SApplicationContext s_AppContext;

		//////////////////////////////////////////////////////////////////////////
		struct PULVIS_API SFrameContext
		{
			SFrameContext();

			uint64_t m_FrameNumber;
			float m_DeltaTime;
		};

		extern PULVIS_API SFrameContext s_FrameContext;

		//////////////////////////////////////////////////////////////////////////
		class PULVIS_API Application
		{
			public:

				Application(const SApplicationSetup& _app_setup);
				~Application();
				
				bool IsCloseRequested() const;
				void Run();
				void FrameCycle();

				const engine::fs::Filesystem& GetEngineFs() const;


			private:

				void InitializeCoreSystems();
				void InitializeWindow();
				void UpdateFrameTime();

				void PreFrame();
				void Frame();
				void PostFrame();

			private:
				
				engine::time::Timer m_FrameTimer;
				float m_FrameTime;
				bool m_bCoreSystemsInitialized;
				bool m_bWindowInitialized;
				
				engine::rendering::CWindow* m_Window;

				engine::fs::Filesystem m_EngineFilesystem;
		};
	}
}
