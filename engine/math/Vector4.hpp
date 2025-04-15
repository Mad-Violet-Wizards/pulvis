#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector4
	{
		public:

		constexpr Vector4()
			: m_X(0)
			, m_Y(0)
			, m_Z(0)
			, m_A(0)
		{
		}

		constexpr Vector4(T _x, T _y, T _z, T _a)
			: m_X(_x)
			, m_Y(_y)
			, m_Z(_z)
			, m_A(_a)
		{
		}

		Vector4(const Vector4<T>& _other)
			: m_X(_other.m_X)
			, m_Y(_other.m_Y)
			, m_Z(_other.m_Z)
			, m_A(_other.m_A)
		{
		}

		Vector4(Vector4<T>&& _other) noexcept
			: m_X(_other.m_X)
			, m_Y(_other.m_Y)
			, m_Z(_other.m_Z)
			, m_A(_other.m_A)
		{
			_other.m_X = 0;
			_other.m_Y = 0;
			_other.m_Z = 0;
			_other.m_A = 0;
		}

		Vector4<T>& operator= (const Vector4<T>& _other)
		{
			m_X = _other.m_X;
			m_Y = _other.m_Y;
			m_Z = _other.m_Z;
			m_A = _other.m_A;
			return *this;
		}

		Vector4<T>& operator= (Vector4<T>&& _other) noexcept
		{
			m_X = _other.m_X;
			m_Y = _other.m_Y;
			m_Z = _other.m_Z;
			m_A = _other.m_A;
			_other.m_X = 0;
			_other.m_Y = 0;
			_other.m_Z = 0;
			_other.m_A = 0;
			return *this;
		}

		template<typename U>
		constexpr Vector4<U> Cast() const
		{
			return Vector4<U>(static_cast<U>(m_X), static_cast<U>(m_Y), static_cast<U>(m_Z), static_cast<U>(m_A));
		}

		T Length() const
		{
			return Sqrt(Pow(m_X, 2) + Pow(m_Y, 2) + Pow(m_Z, 2) + Pow(m_A, 2));
		}

		T LengthSquared() const
		{
			return Pow(m_X, 2) + Pow(m_Y, 2) + Pow(m_Z, 2) + Pow(m_A, 2);
		}

		T Dot(const Vector4<T>& _other) const
		{
			return m_X * _other.m_X + m_Y * _other.m_Y + m_Z * _other.m_Z + m_A * _other.m_A;
		}

		Vector4<T> Cross(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_Y * _other.m_Z - m_Z * _other.m_Y,
							   m_Z * _other.m_X - m_X * _other.m_Z,
							   m_X * _other.m_Y - m_Y * _other.m_X,
							   0 };
		}

		Vector4<T> Normalize() const
		{
			const T length = Length();

			if (length == 0)
				return Vector4<T>{ 0, 0, 0, 0 };

			return Vector4<T>{ m_X / length, m_Y / length, m_Z / length, m_A / length };
		}

	public:

		constexpr Vector4<T> operator+(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_X + _other.m_X, m_Y + _other.m_Y, m_Z + _other.m_Z, m_A + _other.m_A};
		}

		constexpr Vector4<T> operator-(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_X - _other.m_X, m_Y - _other.m_Y, m_Z - _other.m_Z, m_A - _other.m_A };
		}

		constexpr Vector4<T> operator*(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_X * _other.m_X, m_Y * _other.m_Y, m_Z * _other.m_Z, m_A * _other.m_A };
		}

		constexpr Vector4<T> operator/(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_X / _other.m_X, m_Y / _other.m_Y, m_Z / _other.m_Z, m_A / _other.m_A };
		}

		constexpr Vector4<T>& operator+=(const Vector4<T>& _other)
		{
			m_X += _other.m_X;
			m_Y += _other.m_Y;
			m_Z += _other.m_Z;
			m_A += _other.m_A;
			return *this;
		}

		constexpr Vector4<T>& operator-=(const Vector4<T>& _other)
		{
			m_X -= _other.m_X;
			m_Y -= _other.m_Y;
			m_Z -= _other.m_Z;
			m_A -= _other.m_A;
			return *this;
		}

		constexpr Vector4<T>& operator*=(const Vector4<T>& _other)
		{
			m_X *= _other.m_X;
			m_Y *= _other.m_Y;
			m_Z *= _other.m_Z;
			m_A *= _other.m_A;
			return *this;
		}

		constexpr Vector4<T>& operator/=(const Vector4<T>& _other)
		{
			m_X /= _other.m_X;
			m_Y /= _other.m_Y;
			m_Z /= _other.m_Z;
			m_A /= _other.m_A;
			return *this;
		}

		constexpr Vector4<T> operator+(T _scalar) const
		{
			return Vector4<T>{ m_X + _scalar, m_Y + _scalar, m_Z + _scalar, m_A + _scalar };
		}

		constexpr Vector4<T> operator-(T _scalar) const
		{
			return Vector4<T>(m_X - _scalar, m_Y - _scalar, m_Z - _scalar, m_A - _scalar);
		}

		constexpr Vector4<T> operator*(T _scalar) const
		{
			return Vector4<T> { m_X * _scalar, m_Y * _scalar, m_Z * _scalar, m_A * _scalar };
		}

		constexpr Vector4<T> operator/(T _scalar) const
		{
			return Vector4<T>{ m_X / _scalar, m_Y / _scalar, m_Z / _scalar, m_A / _scalar };
		}

		constexpr Vector4<T>& operator+=(T _scalar)
		{
			m_X += _scalar;
			m_Y += _scalar;
			m_Z += _scalar;
			m_A += _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator-=(T _scalar)
		{
			m_X -= _scalar;
			m_Y -= _scalar;
			m_Z -= _scalar;
			m_A -= _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator*=(T _scalar)
		{
			m_X *= _scalar;
			m_Y *= _scalar;
			m_Z *= _scalar;
			m_A *= _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator/=(T _scalar)
		{
			m_X /= _scalar;
			m_Y /= _scalar;
			m_Z /= _scalar;
			m_A /= _scalar;
			return *this;
		}

		constexpr bool operator==(const Vector4<T>& _other) const
		{
			return m_X == _other.m_X && m_Y == _other.m_Y && m_Z == _other.m_Z && m_A == _other.m_A;
		}

		constexpr bool operator!=(const Vector4<T>& _other) const
		{
			return m_X != _other.m_X || m_Y != _other.m_Y || m_Z != _other.m_Z || m_A != _other.m_A;
		}

		constexpr bool operator<(const Vector4<T>& _other) const
		{
			return m_X < _other.m_X && m_Y < _other.m_Y && m_Z < _other.m_Z && m_A < _other.m_A;
		}

		constexpr bool operator<=(const Vector4<T>& _other) const
		{
			return m_X <= _other.m_X && m_Y <= _other.m_Y && m_Z <= _other.m_Z && m_A <= _other.m_A;
		}

		constexpr bool operator>(const Vector4<T>& _other) const
		{
			return m_X > _other.m_X && m_Y > _other.m_Y && m_Z > _other.m_Z && m_A > _other.m_A;
		}

		constexpr bool operator>=(const Vector4<T>& _other) const
		{
			return m_X >= _other.m_X && m_Y >= _other.m_Y && m_Z >= _other.m_Z && m_A >= _other.m_A;
		}

	public:

		T m_X;
		T m_Y;
		T m_Z;
		T m_A;
	};
}