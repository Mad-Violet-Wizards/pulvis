#include "engine/engine_pch.hpp"
#include "ImGuiOpenGLRenderer.hpp"

#include <vendor/common/include/imgui/imgui.h>
#include <vendor/common/include/imgui/backends/imgui_impl_opengl3.h>
#include <vendor/common/include/imgui/backends/imgui_impl_glfw.h>
#include <glfw/glfw3.h>

namespace engine::tools::imgui
{
	void ImGuiOpenGLRenderer::Initialize(GLFWwindow* _window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiOpenGLRenderer::Shutdown()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiOpenGLRenderer::BeginFrame()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiOpenGLRenderer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

