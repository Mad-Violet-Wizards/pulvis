#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector3
	{
	public:

		constexpr Vector3()
			: m_X(0)
			, m_Y(0)
			, m_Z(0)
		{

		}

		constexpr Vector3(T _x, T _y, T _z)
			: m_X(_x)
			, m_Y(_y)
			, m_Z(_z)
		{

		}

		Vector3(const Vector3<T>& _other)
			: m_X(_other.m_X)
			, m_Y(_other.m_Y)
			, m_Z(_other.m_Z)
		{

		}

		Vector3(Vector3<T>&& _other) noexcept
			: m_X(_other.m_X)
			, m_Y(_other.m_Y)
			, m_Z(_other.m_Z)
		{
			_other.m_X = 0;
			_other.m_Y = 0;
			_other.m_Z = 0;
		}

		Vector3<T>& operator= (const Vector3<T>& _other)
		{
			m_X = _other.m_X;
			m_Y = _other.m_Y;
			m_Z = _other.m_Z;
			return *this;
		}

		Vector3<T>& operator= (Vector3<T>&& _other) noexcept
		{
			m_X = _other.m_X;
			m_Y = _other.m_Y;
			m_Z = _other.m_Z;
			_other.m_X = 0;
			_other.m_Y = 0;
			_other.m_Z = 0;
			return *this;
		}

		template<typename U>
		constexpr Vector3<U> Cast() const
		{
			return Vector3<U>(static_cast<U>(m_X), static_cast<U>(m_Y), static_cast<U>(m_Z));
		}

		T Length() const
		{
			return Sqrt(Pow(m_X, 2) + Pow(m_Y, 2) + Pow(m_Z, 2));
		}

		T LengthSquared() const
		{
			return Pow(m_X, 2) + Pow(m_Y, 2) + Pow(m_Z, 2);
		}

		T Dot(const Vector3<T>& _other) const
		{
			return m_X * _other.m_X + m_Y * _other.m_Y + m_Z * _other.m_Z;
		}

		Vector3<T> Cross(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_Y * _other.m_Z - m_Z * _other.m_Y, m_Z * _other.m_X - m_X * _other.m_Z, m_X * _other.m_Y - m_Y * _other.m_X);
		}

		Vector3<T> Normalize() const
		{
			const T length = Length();

			if (length == 0)
				return Vector3<T>(0, 0, 0);

			return Vector3<T>(m_X / length, m_Y / length, m_Z / length);
		}

	public:

		constexpr static Vector3<T> Zero()
		{
			return Vector3<T>(0, 0, 0);
		}

		constexpr static Vector3<T> One()
		{
			return Vector3<T>(1, 1, 1);
		}

		constexpr static Vector3<T> Up()
		{
			return Vector3<T>(0, 1, 0);
		}

		constexpr static Vector3<T> Down()
		{
			return Vector3<T>(0, -1, 0);
		}

		constexpr static Vector3<T> Left()
		{
			return Vector3<T>(-1, 0, 0);
		}

		constexpr static Vector3<T> Right()
		{
			return Vector3<T>(1, 0, 0);
		}

		constexpr static Vector3<T> Forward()
		{
			return Vector3<T>(0, 0, 1);
		}

		constexpr static Vector3<T> Backward()
		{
			return Vector3<T>(0, 0, -1);
		}
	
	public:

		constexpr Vector3<T> operator+(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_X + _other.m_X, m_Y + _other.m_Y, m_Z + _other.m_Z);
		}

		constexpr Vector3<T> operator-(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_X - _other.m_X, m_Y - _other.m_Y, m_Z - _other.m_Z);
		}

		constexpr Vector3<T> operator*(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_X * _other.m_X, m_Y * _other.m_Y, m_Z * _other.m_Z);
		}

		constexpr Vector3<T> operator/(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_X / _other.m_X, m_Y / _other.m_Y, m_Z / _other.m_Z);
		}

		constexpr Vector3<T>& operator+=(const Vector3<T>& _other)
		{
			m_X += _other.m_X;
			m_Y += _other.m_Y;
			m_Z += _other.m_Z;
			return *this;
		}

		constexpr Vector3<T>& operator-=(const Vector3<T>& _other)
		{
			m_X -= _other.m_X;
			m_Y -= _other.m_Y;
			m_Z -= _other.m_Z;
			return *this;
		}

		constexpr Vector3<T>& operator*=(const Vector3<T>& _other)
		{
			m_X *= _other.m_X;
			m_Y *= _other.m_Y;
			m_Z *= _other.m_Z;
			return *this;
		}

		constexpr Vector3<T>& operator/=(const Vector3<T>& _other)
		{
			m_X /= _other.m_X;
			m_Y /= _other.m_Y;
			m_Z /= _other.m_Z;
			return *this;
		}

		constexpr Vector3<T> operator+(T _scalar) const
		{
			return Vector3<T>(m_X + _scalar, m_Y + _scalar, m_Z + _scalar);
		}

		constexpr Vector3<T> operator-(T _scalar) const
		{
			return Vector3<T>(m_X - _scalar, m_Y - _scalar, m_Z - _scalar);
		}

		constexpr Vector3<T> operator*(T _scalar) const
		{
			return Vector3<T>(m_X * _scalar, m_Y * _scalar, m_Z * _scalar);
		}

		constexpr Vector3<T> operator/(T _scalar) const
		{
			return Vector3<T>(m_X / _scalar, m_Y / _scalar, m_Z / _scalar);
		}

		constexpr Vector3<T>& operator+=(T _scalar)
		{
			m_X += _scalar;
			m_Y += _scalar;
			m_Z += _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator-=(T _scalar)
		{
			m_X -= _scalar;
			m_Y -= _scalar;
			m_Z -= _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator*=(T _scalar)
		{
			m_X *= _scalar;
			m_Y *= _scalar;
			m_Z *= _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator/=(T _scalar)
		{
			m_X /= _scalar;
			m_Y /= _scalar;
			m_Z /= _scalar;
			return *this;
		}

		constexpr bool operator==(const Vector3<T>& _other) const
		{
			return m_X == _other.m_X && m_Y == _other.m_Y && m_Z == _other.m_Z;
		}

		constexpr bool operator!=(const Vector3<T>& _other) const
		{
			return m_X != _other.m_X || m_Y != _other.m_Y || m_Z != _other.m_Z;
		}

		constexpr bool operator<(const Vector3<T>& _other) const
		{
			return m_X < _other.m_X && m_Y < _other.m_Y && m_Z < _other.m_Z;
		}

		constexpr bool operator<=(const Vector3<T>& _other) const
		{
			return m_X <= _other.m_X && m_Y <= _other.m_Y && m_Z <= _other.m_Z;
		}

		constexpr bool operator>(const Vector3<T>& _other) const
		{
			return m_X > _other.m_X && m_Y > _other.m_Y && m_Z > _other.m_Z;
		}

		constexpr bool operator>=(const Vector3<T>& _other) const
		{
			return m_X >= _other.m_X && m_Y >= _other.m_Y && m_Z >= _other.m_Z;
		}

	public:
		
		T m_X;
		T m_Y;
		T m_Z;
	};
}