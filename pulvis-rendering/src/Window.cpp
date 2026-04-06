#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.hpp"

namespace pulvis::rendering
{
	CWindow::CWindow(unsigned int _width, unsigned int _height, const char* _title)
		: m_Width(_width)
		, m_Height(_height)
		, m_Title(_title)
	{
		glfwSetErrorCallback([](int error, const char* desc)
			{
				PULVIS_ERROR_LOG("GLFW error.\n {}: {}", error, desc);
			});

		if (!glfwInit())
		{
			PULVIS_ERROR_LOG("Failed to initialize GLFW!");
			return;
		}

#if defined(MAC_OS)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

		if (!m_Window)
		{
			PULVIS_ERROR_LOG("Failed to create GLFW window!");
			glfwTerminate();
			return;
		}

		glfwSetWindowUserPointer(m_Window, this);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height)
			{
				CWindow* window = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(_window));
				window->m_Width = _width;
				window->m_Height = _height;
			});

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
		}

		glViewport(0, 0, _width, _height);
	}

	CWindow::~CWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool CWindow::GetShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
}