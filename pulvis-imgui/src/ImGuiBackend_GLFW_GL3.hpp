#pragma once

#include "ImGuiBackend.hpp"

#include <string>

struct GLFWwindow;

namespace pulvis::imgui
{
		class CImGuiBackend_GLFW_GL3 : public IImGuiBackend
	{
		public:

			explicit CImGuiBackend_GLFW_GL3(GLFWwindow* _window, std::string _glsl_version = "#version 330");
			~CImGuiBackend_GLFW_GL3() override;

			bool Initialize() override;
			void Shutdown() override;

			void BeginFrame() override;
			void Render() override;

		private:
			GLFWwindow* m_Window;
			std::string m_GLSLVersion;
			bool m_Initialized;
		};
}