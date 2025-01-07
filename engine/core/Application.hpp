
#pragma once

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

		extern PULVIS_API SApplicationContext s_AppContext;

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

				void InitializeServices();
				void UpdateFrameTime();

				void PreFrame();
				void Frame();
				void PostFrame();

			private:
				
				engine::time::Timer m_FrameTimer;
				float m_FrameTime;
				bool m_bCoreSystemsInitialized;

				engine::fs::Filesystem m_EngineFilesystem;
		};
	}
}
