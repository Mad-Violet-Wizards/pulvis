
#pragma once

#include "engine/rendering/Window.hpp"
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
				~Application() = default;
				
				bool IsCloseRequested() const;
				void Run();

				void StartFrame();
				void Frame_InputUpdate();
				void Frame_UpdateSceneStateMachine();
				void FinishFrame();

			private:

				void InitializeCoreSystems();
				void UpdateFrameTime();

			private:
				
				engine::time::Timer m_FrameTimer;
				float m_FrameTime;
				bool m_bCoreSystemsInitialized;
				
				engine::rendering::CWindow m_Window;
		};
	}
}
