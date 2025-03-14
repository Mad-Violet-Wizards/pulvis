
#pragma once

#include "ApplicationContext.hpp"
#include "engine/time/Timer.hpp"
#include "Setup.hpp"

namespace engine
{
	namespace core
	{
		//////////////////////////////////////////////////////////////////////////
		class Application
		{
			public:

				Application(const SApplicationSetup& _app_setup);
				~Application();
				
				bool IsCloseRequested() const;
				void Run();

#ifdef DEBUG
				void SimulateFrame();
#endif

				static const CApplicationContext& GetContext() { return Application::m_AppContext; }

			private:

				void UpdateFrameTime();

				void PreFrame();
				void Frame();
				void PostFrame();

			private:
				
				static CApplicationContext m_AppContext;
				engine::time::Timer m_FrameTimer;
				float m_FrameTime;
		};
	}
}
