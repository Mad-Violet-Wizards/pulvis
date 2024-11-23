#include "engine/engine_pch.hpp"
#include "Window.hpp"
#include <format>
#include <fmt/format.h>

#include <GLFW/glfw3.h>

namespace engine
{
namespace rendering
{
	CWindow::CWindow(unsigned int _width, unsigned int _height, const char* _title)
		: m_Width(_width)
		, m_Height(_height)
		, m_Title(_title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

		if (!m_Window)
		{
			ASSERT(false, "Failed to create window.");
			glfwTerminate();
		}

		glfwSetWindowUserPointer(m_Window, this);

		glfwSetErrorCallback([](int error, const char* desc)
		{
				std::cout << fmt::format("GLFW Error {0}: {1}\n", error, desc);
		});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height)
		{
				CWindow* window = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(_window));
				window->m_Width = _width;
				window->m_Height = _height;
		});
	}

	CWindow::~CWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool CWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

}
}
