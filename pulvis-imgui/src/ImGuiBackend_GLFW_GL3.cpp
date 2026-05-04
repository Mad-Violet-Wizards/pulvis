#include "ImGuiBackend_GLFW_GL3.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace pulvis::imgui
{
	CImGuiBackend_GLFW_GL3::CImGuiBackend_GLFW_GL3(GLFWwindow* _window, std::string _glsl_version)
		: m_Window(_window)
		, m_GLSLVersion(std::move(_glsl_version))
		, m_Initialized(false) {}

	CImGuiBackend_GLFW_GL3::~CImGuiBackend_GLFW_GL3()
	{
		if (m_Initialized)
		{
			Shutdown();
		}
	}

	bool CImGuiBackend_GLFW_GL3::Initialize()
	{
		if (m_Initialized)
		{
			return true;
		}

		if (!ImGui_ImplGlfw_InitForOpenGL(m_Window, true))
		{
			return false;
		}

		if (!ImGui_ImplOpenGL3_Init(m_GLSLVersion.c_str()))
		{
			ImGui_ImplGlfw_Shutdown();
			return false;
		}

		m_Initialized = true;
		return true;
	}

	void CImGuiBackend_GLFW_GL3::Shutdown()
	{
		if (!m_Initialized)
			return;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		m_Initialized = false;
	}

	void CImGuiBackend_GLFW_GL3::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void CImGuiBackend_GLFW_GL3::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}