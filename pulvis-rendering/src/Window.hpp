#pragma once

struct GLFWwindow;

namespace pulvis::rendering
{
	class CWindow
	{
		public:

			CWindow(unsigned int _width, unsigned int _height, const char* _title);
			~CWindow();

			CWindow(const CWindow&) = delete;
			CWindow& operator=(const CWindow&) = delete;

			bool GetShouldClose() const;

			GLFWwindow* GetNativeWindow() { return m_Window; }

		private:

			unsigned int m_Width;
			unsigned int m_Height;
			const char* m_Title;
			GLFWwindow* m_Window;
	};
}