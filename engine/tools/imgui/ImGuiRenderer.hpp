#pragma once

#include <vendor/common/include/imgui/imgui.h>

struct GLFWwindow;

namespace engine::tools::imgui
{
	class IImGuiRenderer
	{
		public:

			virtual ~IImGuiRenderer() = default;
			virtual void Initialize(GLFWwindow* _window) = 0;
			virtual void Shutdown() = 0;
			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;
	};
}