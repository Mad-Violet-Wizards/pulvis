#include <iostream>
#include "RenderSevice.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr float vertices[] =
{
    // coordiantes       //color         // texture coordinates
     0.5f,   0.5f, 0.f,  1.f, 0.f, 0.f,  1.f, 1.f,
     0.5f,  -0.5f, 0.f,  0.f, 1.f, 0.f,  1.f, 0.f,
    -0.5f,  -0.5f, 0.f,  0.f, 0.f, 1.f,  0.f, 0.f,
    -0.5f,   0.5f, 0.f,  1.f, 1.f, 0.f,  0.f, 1.f
};

constexpr unsigned int indices[] =
{
  0, 1, 3,
  1, 2, 3
};

int main() {
    std::cout << "Hello playground rendering executable\n";

    pulvis::rendering::CRenderService& render_service = pulvis::rendering::CRenderService::GetInstance();
    render_service.Initialize(pulvis::rendering::ERendererType::OpenGL);

    while (!render_service.GetShouldClose())
    {
      render_service.Frame();
    }

  //  pulvis::rendering::CWindow window(800, 600, "Test");

  //  pulvis::rendering::gl::CGLShader shader("shaders/texture_shader.vert", "shaders/texture_shader.frag");

  //  //////////////////////////////////////////////////////////////////////
  //  // Setup vertex data (and buffers) and configure vertex attrs.
  //  unsigned int VBO, VAO, EBO;
  //  glGenVertexArrays(1, &VAO);
  //  glGenBuffers(1, &VBO);
  //  glGenBuffers(1, &EBO);

  //  glBindVertexArray(VAO);

  //  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  //  glEnableVertexAttribArray(0);

  //  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  //  glEnableVertexAttribArray(1);

  //  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  //  glEnableVertexAttribArray(2);

  //  //////////////////////////////////////////////////////////////////////
  //  // Setup texture
  //  unsigned int texture1, texture2;
  //  glGenTextures(1, &texture1);
  //  glBindTexture(GL_TEXTURE_2D, texture1);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //  int width, height, nChannels;
  //  //unsigned char* data = nullptr;
  //  unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nChannels, 0);
  //  if (data)
  //  {
  //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  //    glGenerateMipmap(GL_TEXTURE_2D);
  //  }
  //  else
  //  {
  //    std::cout << "Failed to load texture!\n";
  //  }
		//stbi_image_free(data);

  //  // Setup texture 2
  //  glGenTextures(1, &texture2);
  //  glBindTexture(GL_TEXTURE_2D, texture2);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  stbi_set_flip_vertically_on_load(true);
  //  data = stbi_load("assets/awesomeface.png", &width, &height, &nChannels, 0);
  //  if (data)
  //  {
  //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  //    glGenerateMipmap(GL_TEXTURE_2D);
  //  }
  //  else
  //  {
  //    std::cout << "Failed to load texture!\n";
  //  }
  //  stbi_image_free(data);

  //  shader.Use();
  //  glUniform1d(glGetUniformLocation(shader.GetID(), "texture1"), 0);
  //  shader.SetInt("texture2", 1);


  //  while (!window.ShouldClose())
  //  {
  //    ProcessInput(window.GetNativeWindow());

  //    glClearColor(0.f, 0.f, 0.1f, 1.f);
  //    glClear(GL_COLOR_BUFFER_BIT);

  //    glActiveTexture(GL_TEXTURE0);
  //    glBindTexture(GL_TEXTURE_2D, texture1);

  //    glActiveTexture(GL_TEXTURE1);
  //    glBindTexture(GL_TEXTURE_2D, texture2);

  //    shader.SetFloat("mixValue", mixValue);

  //    shader.Use();
  //    glBindVertexArray(VAO);
  //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  //    glfwSwapBuffers(window.GetNativeWindow());
  //    glfwPollEvents();
  //  }

  //  glDeleteVertexArrays(1, &VAO);
  //  glDeleteBuffers(1, &VBO);
  //  glDeleteBuffers(2, &EBO);

		return 0;
}
