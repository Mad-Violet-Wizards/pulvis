#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector2
	{
		public:

			constexpr Vector2()
			{

			}

			constexpr Vector2(T _x, T _y)
			{
				m_v[0] = _x;
				m_v[1] = _y;
			}

			Vector2(const Vector2<T>& _other) = default;
			Vector2(Vector2<T>&& _other) noexcept = default;
			Vector2<T>& operator= (const Vector2<T>& _other) = default;
			Vector2<T>& operator= (Vector2<T>&& _other) = default;

			template<typename U>
			constexpr Vector2<U> Cast() const
			{
				return Vector2<U>(static_cast<U>(m_v[0]), static_cast<U>(m_v[1]));
			}

			T Length() const
			{
				return std::sqrt(std::pow(m_v[0], 2) + std::pow(m_v[1], 2));
			}

			T LengthSquared() const
			{
				return std::pow(m_v[0], 2) + std::pow(m_v[1], 2);
			}

			T Dot(const Vector2<T>& _other) const
			{
				return m_v[0] * _other[0] + m_v[1] * _other[1];
			}

			T Cross(const Vector2<T>& _other) const
			{
				return m_v[0] * _other[1] - m_v[1] * _other[0];
			}

			Vector2<T> Normalize() const
			{
				const T length = Length();

				if (length == 0)
					return { 0, 0 };

				return { m_v[0] / length, m_v[1] / length };
			}

		public:

			constexpr static Vector2<T> Zero()
			{
				return Vector2<T>(0, 0);
			}

			constexpr static Vector2<T> One()
			{
				return Vector2<T>(1, 1);
			}

			constexpr static Vector2<T> Up()
			{
				return Vector2<T>(0, 1);
			}

			constexpr static Vector2<T> Down()
			{
				return Vector2<T>(0, -1);
			}

			constexpr static Vector2<T> Left()
			{
				return Vector2<T>(-1, 0);
			}

			constexpr static Vector2<T> Right()
			{
				return Vector2<T>(1, 0);
			}

		public:

			T operator[](size_t _index) const
			{
				return m_v[_index];
			}

			T& operator[](size_t _index)
			{
				return m_v[_index];
			}

			constexpr Vector2<T> operator+(const Vector2<T>& _other) const
			{
				return { m_v[0] + _other[0], m_v[1] + _other[1] };
			}

			constexpr Vector2<T> operator-(const Vector2<T>& _other) const
			{
				return { m_v[0] - _other[0], m_v[1] - _other[1] };
			}

			constexpr Vector2<T> operator*(const Vector2<T>& _other) const
			{
				return { m_v[0] * _other[0], m_v[1] * _other[1] };
			}

			constexpr Vector2<T> operator/(const Vector2<T>& _other) const
			{
				return { m_v[0] / _other[0], m_v[1] / _other[1] };
			}

			constexpr Vector2<T>& operator+=(const Vector2<T>& _other)
			{
				m_v[0] += _other[0];
				m_v[1] += _other[1];
				return *this;
			}

			constexpr Vector2<T>& operator-=(const Vector2<T>& _other)
			{
				m_v[0] -= _other[0];
				m_v[1] -= _other[1];
				return *this;
			}

			constexpr Vector2<T>& operator*=(const Vector2<T>& _other)
			{
				m_v[0] *= _other[0];
				m_v[1] *= _other[1];
				return *this;
			}

			constexpr Vector2<T>& operator/=(const Vector2<T>& _other)
			{
				m_v[0] /= _other[0];
				m_v[1] /= _other[1];
				return *this;
			}

			constexpr Vector2<T> operator+(T _scalar) const
			{
				return { m_v[0] + _scalar, m_v[1] + _scalar };
			}

			constexpr Vector2<T> operator-(T _scalar) const
			{
				return { m_v[0] - _scalar, m_v[1] - _scalar };
			}

			constexpr Vector2<T> operator*(T _scalar) const
			{
				return { m_v[0] * _scalar, m_v[1] * _scalar };
			}

			constexpr Vector2<T> operator/(T _scalar) const
			{
				return { m_v[0] / _scalar, m_v[1] / _scalar };
			}

			constexpr Vector2<T>& operator+=(T _scalar)
			{
				m_v[0] += _scalar;
				m_v[1] += _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator-=(T _scalar)
			{
				m_v[0] -= _scalar;
				m_v[1] -= _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator*=(T _scalar)
			{
				m_v[0] *= _scalar;
				m_v[1] *= _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator/=(T _scalar)
			{
				m_v[0] /= _scalar;
				m_v[1] /= _scalar;
				return *this;
			}

			constexpr bool operator==(const Vector2<T>& _other) const
			{
				return m_v[0] == _other[0] && m_v[1] == _other[1];
			}

			constexpr bool operator!=(const Vector2<T>& _other) const
			{
				return m_v[0] != _other[0] || m_v[1] != _other[1];
			}

			constexpr bool operator<(const Vector2<T>& _other) const
			{
				return m_v[0] < _other[0] && m_v[1] < _other[1];
			}

			constexpr bool operator<=(const Vector2<T>& _other) const
			{
				return m_v[0] <= _other[0] && m_v[1] <= _other[1];
			}

			constexpr bool operator>(const Vector2<T>& _other) const
			{
				return m_v[0] > _other[0] && m_v[1] > _other[1];
			}

			constexpr bool operator>=(const Vector2<T>& _other) const
			{
				return m_v[0] >= _other[0] && m_v[1] >= _other[1];
			}

		public:

			std::array<T, 2> m_v;
	};
}