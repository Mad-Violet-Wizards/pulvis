#include "engine/engine_pch.hpp"
#include "RendererOpenGL.hpp"
#include "ContextOpenGL.hpp"
#include "engine/core/Application.hpp"
#include "Shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine::rendering::opengl
{
	RendererOpenGL::RendererOpenGL()
		: engine::rendering::IRenderer()
		, m_Window(nullptr)
		, m_Context(nullptr)
		, m_Initialized(false)
	{

	}

	RendererOpenGL::~RendererOpenGL()
	{

	}

	void RendererOpenGL::Initialize()
	{
		static const engine::core::CApplicationContext& app_context = engine::core::Application::GetContext();

		const std::string window_name = app_context.m_Setup.m_ApplicationName;

		m_Window = new engine::rendering::CWindow(app_context.m_Setup.m_WindowWidth,
			app_context.m_Setup.m_WindowHeight,
			window_name.c_str());

		m_Context = new CContext(m_Window);
		// Initialize OpenGL

		m_Initialized = true;
	}

	void RendererOpenGL::BeginFrame()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererOpenGL::Frame()
	{
		CShader* base_shader = m_Context->FindShader("triangle");

		if (base_shader)
		{
			float triangleVert[] = {
				0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.0f,
				0.5f, 0.0f, 0.0f
			};

			glViewport(0, 0, 800, 600);

			GLuint tempVAO, tempVBO;
			glGenVertexArrays(1, &tempVAO);
			glGenBuffers(1, &tempVBO);

			glBindVertexArray(tempVAO);
			glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVert), triangleVert, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			base_shader->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			glDeleteVertexArrays(1, &tempVAO);
			glDeleteBuffers(1, &tempVBO);
		}
	}

	void RendererOpenGL::EndFrame()
	{
		glfwSwapBuffers(m_Window->GetWindow());
		glfwPollEvents();
	}

	void RendererOpenGL::Shutdown()
	{

	}

	bool RendererOpenGL::ShouldClose() const
	{
		if (m_Window)
		{
			return m_Window->ShouldClose();
		}

		return false;
	}

	void RendererOpenGL::SetupShaders(const std::vector<CShader*>& _shaders_vec)
	{
		m_Context->SetupShaders(_shaders_vec);
	}
}