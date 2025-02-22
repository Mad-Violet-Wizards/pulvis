#pragma once

namespace engine
{
namespace core
{
	enum class EClientApp
	{
		Unknown,
		Game,
		Playground,
		Editor
	};

	struct SApplicationSetup
	{
		char* m_ApplicationName;
		char* m_AplicationToRun;
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;

#if defined DEBUG
		bool m_AssertionsEnabled = true;
#else
		bool m_AssertionsEnabled = false;
#endif

		EClientApp m_ClientApp = EClientApp::Unknown;
	};
}
}