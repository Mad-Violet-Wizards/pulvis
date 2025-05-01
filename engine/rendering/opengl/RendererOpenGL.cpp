#include "engine/engine_pch.hpp"
#include "RendererOpenGL.hpp"
#include "ContextOpenGL.hpp"
#include "engine/core/Application.hpp"
#include "ShaderOpenGL.hpp"
#include "TextureOpenGL.hpp"
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

		m_Initialized = true;
	}

	void RendererOpenGL::BeginFrame()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererOpenGL::Frame()
	{
		CShaderOpenGL* base_shader = m_Context->FindShader("triangle");
		CTextureOpenGL* empty_texture = m_Context->FindTexture("hamster");

		if (base_shader && empty_texture)
		{
			float vertices[] = {
				// positions          // colors           // texture coords
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
			};
			unsigned int indices[] = {
					0, 1, 3, // first triangle
					1, 2, 3  // second triangle
			};
			unsigned int VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture coord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			glActiveTexture(GL_TEXTURE0);
			empty_texture->BindTexture();

			base_shader->Bind();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
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

	void RendererOpenGL::SetupShaders(std::vector<CShaderOpenGL*>&& _shaders_vec)
	{
		m_Context->SetupShaders(std::move(_shaders_vec));
	}
	void RendererOpenGL::SetupTextures(std::vector<CTextureOpenGL*>&& _textures_vec)
	{
		m_Context->SetupTextures(std::move(_textures_vec));
	}
}