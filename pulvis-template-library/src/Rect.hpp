#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <algorithm>

/*
Rect.hpp
{ x, y } - Coordinates of the rectangle's top-left corner.
*/

namespace pulvis::tl
{
    template<typename T>
    class Rect
    {
        public:

            constexpr Rect() = default;
            constexpr Rect(T _x, T _y, T _width, T _height)
                : Position(_x, _y)
                , Size(_width, _height)
            {
            }

            constexpr Rect(const glm::vec<2, T>& _position, 
                           const glm::vec<2, T>& _size)
                : Position(_position)
                , Size(_size)
            {
            }

            template<typename U>
            constexpr operator Rect<U>() const
            {
                return Rect<U>(static_cast<U>(Position.x), 
                               static_cast<U>(Position.y), 
                               static_cast<U>(Size.x), 
                               static_cast<U>(Size.y));
						}

            constexpr bool ContainsPoint(const glm::vec<2, T>& _point) const
            {
							return _point.x >= Position.x &&
								_point.x <= Position.x + Size.x &&
								_point.y >= Position.y &&
								_point.y <= Position.y + Size.y;
            }

            constexpr bool ContainsRect(const Rect<T> _rect) const
            {
              return ContainsPoint(_rect.Position) && ContainsPoint(_rect.Position + _rect.Size);
            }

            constexpr std::optional<glm::vec<2, T>> IntersectPoint(const Rect<T>& _other) const
            {
              const T x1 = std::max(Position.x, _other.Position.x);
              const T y1 = std::max(Position.y, _other.Position.y);
              const T x2 = std::min(Position.x + Size.x, _other.Position.x + _other.Size.x);
              const T y2 = std::min(Position.y + Size.y, _other.Position.y + _other.Size.y);

              if (x1 < x2 && y1 < y2)
              {
                return glm::vec<2, T>(x1, y1);
              }

              return std::nullopt; // No intersection
            }

            constexpr bool Intersect(const Rect<T>& _other) const
            {
              return !(Position.x > _other.Position.x + _other.Size.x
                || Position.x + Size.x < _other.Position.x
                || Position.y > _other.Position.y + _other.Size.y
                || Position.y + Size.y < _other.Position.y);
            }

            constexpr glm::vec<2, T> GetCenter() const
            {
              return (Position + Size) / static_cast<T>(2);
						}


        public:

          glm::vec<2, T> Position{ 0, 0 };
          glm::vec<2, T> Size{ 0, 0 };
    };
}
