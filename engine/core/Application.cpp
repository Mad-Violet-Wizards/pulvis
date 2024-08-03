#pragma once
#include "engine/engine_pch.hpp"
#include "Application.hpp"
#include "Setup.hpp"

namespace engine
{
	namespace core
	{
		Application::Application(const ApplicationSetup& _app_setup)
		{
			// Initialize assertions
			Singleton<CAssertManager>::Init();
			// Initialize filesystems
			// Initialize logging
		}
	}
}