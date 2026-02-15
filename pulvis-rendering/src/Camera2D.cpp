#include "Camera2D.hpp"

namespace pulvis::rendering
{
	glm::mat4 CCamera2D::GetViewMatrix() const
	{
    glm::mat4 view(1.0f);

    view = glm::translate(view, glm::vec3(-m_Position, 0.0f));

    if (m_Rotation != 0.0f)
    {
      view = glm::translate(view, glm::vec3(0.5f * m_ViewportWidth, 0.5f * m_ViewportHeight, 0.0f));
      view = glm::rotate(view, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
      view = glm::translate(view, glm::vec3(-0.5f * m_ViewportWidth, -0.5f * m_ViewportHeight, 0.0f));
    }

    return view;
	}

  glm::mat4 CCamera2D::GetProjectionMatrix() const
  {
    const float w = m_ViewportWidth / m_Zoom;
    const float h = m_ViewportHeight / m_Zoom;

    return glm::ortho(0.0f, w, 0.0f, h);
  }

  glm::mat4 CCamera2D::GetViewProjMatrix() const
  {
    return GetProjectionMatrix() * GetViewMatrix();
  }

  glm::vec2 CCamera2D::ScreenToWorld(const glm::vec2& screen) const
  {
    return glm::vec2(screen.x / m_Zoom, screen.y / m_Zoom) + m_Position;
  }

  glm::vec2 CCamera2D::WorldToScreen(const glm::vec2& world) const
  {
    glm::vec2 local = world - m_Position;
    return local * m_Zoom;
  }


}