#pragma 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace engine
{
namespace rendering
{
	class Window
	{
		public:

			Window(unsigned int _width, unsigned int _height, const char* _title);
			~Window();

			Window(const Window&) = delete;
			Window &operator=(const Window&) = delete;

			bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }

		private:

			unsigned int m_Width;
			unsigned int m_Height;
			const char* m_Title;
			GLFWwindow *m_Window;

	};
}
}