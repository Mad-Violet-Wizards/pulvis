#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector2
	{
		public:

			constexpr Vector2()
				: m_X(0)
				, m_Y(0)
			{

			}

			constexpr Vector2(T _x, T _y)
				: m_X(_x)
				, m_Y(_y)
			{

			}

			Vector2(const Vector2<T>& _other)
				: m_X(_other.m_X)
				, m_Y(_other.m_Y)
			{

			}

			Vector2(Vector2<T>&& _other) noexcept
				: m_X(_other.m_X)
				, m_Y(_other.m_Y)
			{
				_other.m_X = 0;
				_other.m_Y = 0;
			}

			Vector2<T>& operator= (const Vector2<T>& _other)
			{
				m_X = _other.m_X;
				m_Y = _other.m_Y;
				return *this;
			}

			Vector2<T>& operator= (Vector2<T>&& _other) noexcept
			{
				m_X = _other.m_X;
				m_Y = _other.m_Y;
				_other.m_X = 0;
				_other.m_Y = 0;
				return *this;
			}

			template<typename U>
			constexpr Vector2<U> Cast() const
			{
				return Vector2<U>(static_cast<U>(m_X), static_cast<U>(m_Y));
			}

			T Length() const
			{
				return Sqrt(Pow(m_X, 2) + Pow(m_Y, 2));
			}

			T LengthSquared() const
			{
				return Pow(m_X, 2) + Pow(m_Y, 2);
			}

			T Dot(const Vector2<T>& _other) const
			{
				return m_X * _other.m_X + m_Y * _other.m_Y;
			}

			T Cross(const Vector2<T>& _other) const
			{
				return m_X * _other.m_Y - m_Y * _other.m_X;
			}

			Vector2<T> Normalize() const
			{
				const T length = Length();

				if (length == 0)
					return { 0, 0 };

				return { m_X / length, m_Y / length };
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

			constexpr Vector2<T> operator+(const Vector2<T>& _other) const
			{
				return { m_X + _other.m_X, m_Y + _other.m_Y };
			}

			constexpr Vector2<T> operator-(const Vector2<T>& _other) const
			{
				return { m_X - _other.m_X, m_Y - _other.m_Y };
			}

			constexpr Vector2<T> operator*(const Vector2<T>& _other) const
			{
				return { m_X * _other.m_X, m_Y * _other.m_Y };
			}

			constexpr Vector2<T> operator/(const Vector2<T>& _other) const
			{
				return { m_X / _other.m_X, m_Y / _other.m_Y };
			}

			constexpr Vector2<T>& operator+=(const Vector2<T>& _other)
			{
				m_X += _other.m_X;
				m_Y += _other.m_Y;
				return *this;
			}

			constexpr Vector2<T>& operator-=(const Vector2<T>& _other)
			{
				m_X -= _other.m_X;
				m_Y -= _other.m_Y;
				return *this;
			}

			constexpr Vector2<T>& operator*=(const Vector2<T>& _other)
			{
				m_X *= _other.m_X;
				m_Y *= _other.m_Y;
				return *this;
			}

			constexpr Vector2<T>& operator/=(const Vector2<T>& _other)
			{
				m_X /= _other.m_X;
				m_Y /= _other.m_Y;
				return *this;
			}

			constexpr Vector2<T> operator+(T _scalar) const
			{
				return { m_X + _scalar, m_Y + _scalar };
			}

			constexpr Vector2<T> operator-(T _scalar) const
			{
				return { m_X - _scalar, m_Y - _scalar };
			}

			constexpr Vector2<T> operator*(T _scalar) const
			{
				return { m_X * _scalar, m_Y * _scalar };
			}

			constexpr Vector2<T> operator/(T _scalar) const
			{
				return { m_X / _scalar, m_Y / _scalar };
			}

			constexpr Vector2<T>& operator+=(T _scalar)
			{
				m_X += _scalar;
				m_Y += _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator-=(T _scalar)
			{
				m_X -= _scalar;
				m_Y -= _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator*=(T _scalar)
			{
				m_X *= _scalar;
				m_Y *= _scalar;
				return *this;
			}

			constexpr Vector2<T>& operator/=(T _scalar)
			{
				m_X /= _scalar;
				m_Y /= _scalar;
				return *this;
			}

			constexpr bool operator==(const Vector2<T>& _other) const
			{
				return m_X == _other.m_X && m_Y == _other.m_Y;
			}

			constexpr bool operator!=(const Vector2<T>& _other) const
			{
				return m_X != _other.m_X || m_Y != _other.m_Y;
			}

			constexpr bool operator<(const Vector2<T>& _other) const
			{
				return m_X < _other.m_X && m_Y < _other.m_Y;
			}

			constexpr bool operator<=(const Vector2<T>& _other) const
			{
				return m_X <= _other.m_X && m_Y <= _other.m_Y;
			}

			constexpr bool operator>(const Vector2<T>& _other) const
			{
				return m_X > _other.m_X && m_Y > _other.m_Y;
			}

			constexpr bool operator>=(const Vector2<T>& _other) const
			{
				return m_X >= _other.m_X && m_Y >= _other.m_Y;
			}

		public:

			T m_X;
			T m_Y;
	};
}