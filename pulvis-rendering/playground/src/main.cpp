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

		return 0;
}
