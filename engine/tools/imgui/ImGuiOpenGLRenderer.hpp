#pragma once

struct GLFWwindow;

#include <vendor/common/include/imgui/imgui.h>
#include "ImGuiRenderer.hpp"

namespace engine::tools::imgui
{
	class ImGuiOpenGLRenderer : public IImGuiRenderer
	{
		public:

			void Initialize(GLFWwindow* _window) override;
			void Shutdown() override;
			void BeginFrame() override;
			void EndFrame() override;
	};
}