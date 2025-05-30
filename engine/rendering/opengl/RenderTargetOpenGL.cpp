#include "engine/engine_pch.hpp"
#include "RenderTargetOpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine::rendering::opengl
{
	CRenderTargetOpenGL::CRenderTargetOpenGL(int _buffer_id)
		: m_FramebufferID(_buffer_id)
	{
	}
	void CRenderTargetOpenGL::Clear(const engine::math::Vector4<float>& _color)
	{
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void engine::rendering::opengl::CRenderTargetOpenGL::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
	}
}