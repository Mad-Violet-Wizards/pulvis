#pragma once

#define GLFW_INCLUDE_VULKAN

struct GLFWwindow;

namespace engine::rendering
{
	class CWindow
	{
		public:

			CWindow(unsigned int _width, unsigned int _height, const char* _title);
			~CWindow();

			CWindow(const CWindow&) = delete;
			CWindow &operator=(const CWindow&) = delete;

			bool ShouldClose() const;

			GLFWwindow* GetWindow() { return m_Window; }

		private:

			unsigned int m_Width;
			unsigned int m_Height;
			const char* m_Title;
			GLFWwindow *m_Window;

	};
}