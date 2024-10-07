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
		std::string m_WindowName = "Application name (Pulvis 1.0.0)";
		std::string m_InitProject = "";
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;

		EClientApp m_ClientApp = EClientApp::Unknown;
	};
}
}