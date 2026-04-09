#include "Camera2D.hpp"

namespace pulvis::rendering
{
	glm::mat4 CCamera2D::GetViewMatrix() const
	{
    glm::mat4 view(1.0f);

    view = glm::translate(view, glm::vec3(-m_Position, 0.0f));

    if (m_Rotation != 0.0f)
    {
      view = glm::rotate(view, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    return view;
	}

  glm::mat4 CCamera2D::GetProjectionMatrix() const
  {
    const float half_w = (m_ViewportWidth * 0.5f) / m_Zoom;
    const float half_h = (m_ViewportHeight * 0.5f) / m_Zoom;

    return glm::ortho(-half_w, half_w, -half_h, half_h);
  }

  glm::mat4 CCamera2D::GetViewProjMatrix() const
  {
    return GetProjectionMatrix() * GetViewMatrix();
  }

  glm::vec2 CCamera2D::ScreenToWorld(const glm::vec2& screen) const
  {
    const float half_w = m_ViewportWidth * 0.5f;
    const float half_h = m_ViewportHeight * 0.5f;

    return glm::vec2(
      (screen.x - half_w) / m_Zoom + m_Position.x,
      (half_h - screen.y) / m_Zoom + m_Position.y
    );
  }

  glm::vec2 CCamera2D::WorldToScreen(const glm::vec2& world) const
  {
    const float half_w = m_ViewportWidth * 0.5f;
    const float half_h = m_ViewportHeight * 0.5f;

    glm::vec2 local = (world - m_Position) * m_Zoom;
    return glm::vec2(local.x + half_w, half_h - local.y);
  }
}