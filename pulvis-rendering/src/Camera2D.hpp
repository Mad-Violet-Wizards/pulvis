#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace pulvis::rendering
{
  class CCamera2D
  {
  public:
    CCamera2D(float viewport_width, float viewport_height)
      : m_Position(0.0f, 0.0f)
      , m_Zoom(1.0f)
      , m_Rotation(0.0f)
      , m_ViewportWidth(viewport_width)
      , m_ViewportHeight(viewport_height)
    {
    }

    void SetPosition(const glm::vec2& _position) { m_Position = _position; }
    void Move(const glm::vec2& _delta) { m_Position += _delta; }

    void SetZoom(float zoom)
    {
      m_Zoom = glm::clamp(zoom, m_MinZoom, m_MaxZoom);
    }

    void AddZoom(float delta_zoom)
    {
      SetZoom(m_Zoom + delta_zoom);
    }

    void SetRotation(float radians) { m_Rotation = radians; }
    void AddRotation(float delta) { m_Rotation += delta; }

    void SetViewportSize(float width, float height)
    {
      m_ViewportWidth = width;
      m_ViewportHeight = height;
    }

    const glm::vec2& GetPosition() const { return m_Position; }
    float GetZoom()        const { return m_Zoom; }
    float GetRotation()    const { return m_Rotation; }
    float GetViewportWidth()  const { return m_ViewportWidth; }
    float GetViewportHeight() const { return m_ViewportHeight; }

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix() const;

    glm::mat4 GetViewProjMatrix() const;

    glm::vec2 ScreenToWorld(const glm::vec2& screen) const;

    glm::vec2 WorldToScreen(const glm::vec2& world) const;

  private:
    glm::vec2 m_Position;
    float     m_Zoom;
    float     m_Rotation;

    float     m_ViewportWidth;
    float     m_ViewportHeight;

    float     m_MinZoom = 0.1f;
    float     m_MaxZoom = 10.0f;
  };
}