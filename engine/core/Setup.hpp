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

	struct PULVIS_API SApplicationSetup
	{
		char* m_ApplicationName;
		char* m_AplicationToRun;
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;

		EClientApp m_ClientApp = EClientApp::Unknown;
	};
}
}