#pragma once

#define GLFW_INCLUDE_VULKAN

struct GLFWwindow;

namespace engine
{
namespace rendering
{
	class PULVIS_API CWindow
	{
		public:

			CWindow(unsigned int _width, unsigned int _height, const char* _title);
			~CWindow();

			CWindow(const CWindow&) = delete;
			CWindow &operator=(const CWindow&) = delete;

			bool ShouldClose() const;

		private:

			unsigned int m_Width;
			unsigned int m_Height;
			const char* m_Title;
			GLFWwindow *m_Window;

	};
}
}