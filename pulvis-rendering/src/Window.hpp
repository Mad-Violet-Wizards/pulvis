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

			unsigned int GetWidth() const { return m_Width; }
			unsigned int GetHeight() const { return m_Height; }

			unsigned int GetFramebufferWidth() const { return m_FramebufferWidth; }
			unsigned int GetFramebufferHeight() const { return m_FramebufferHeight; }

			bool IsKeyPressed(int key) const;

		private:

			unsigned int m_Width;
			unsigned int m_Height;
			unsigned int m_FramebufferWidth;
			unsigned int m_FramebufferHeight;
			const char* m_Title;
			GLFWwindow* m_Window;
	};
}