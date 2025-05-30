#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector3
	{
	public:

		constexpr Vector3() = default;

		constexpr Vector3(T _x, T _y, T _z)
		{
			m_v[0] = _x;
			m_v[1] = _y;
			m_v[2] = _z;
		}

		Vector3(const Vector3<T>& _other) = default;
		Vector3(Vector3<T>&& _other) noexcept = default;
		Vector3<T>& operator= (const Vector3<T>& _other) = default;
		Vector3<T>& operator= (Vector3<T>&& _other) noexcept = default;

		template<typename U>
		constexpr Vector3<U> Cast() const
		{
			return Vector3<U>(static_cast<U>(m_v[0]), static_cast<U>(m_v[1]), static_cast<U>(m_v[2]));
		}

		T Length() const
		{
			return std::sqrt(std::pow(m_v[0], 2) + std::pow(m_v[1], 2) + std::pow(m_v[2], 2));
		}

		T LengthSquared() const
		{
			return std::pow(m_v[0], 2) + std::pow(m_v[1], 2) + std::pow(m_v[2], 2);
		}

		T Dot(const Vector3<T>& _other) const
		{
			return m_v[0] * _other[0] + m_v[1] * _other[1] + m_v[2] * _other[2];
		}

		Vector3<T> Cross(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_v[1] * _other[2] - m_v[2] * _other[1], m_v[2] * _other[0] - m_v[0] * _other[2], m_v[0] * _other[1] - m_v[1] * _other[0]);
		}

		Vector3<T> Normalize() const
		{
			const T length = Length();

			if (length == 0)
				return Vector3<T>(0, 0, 0);

			return Vector3<T>(m_v[0] / length, m_v[1] / length, m_v[2] / length);
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

		T operator[](size_t _index) const
		{
			return m_v[_index];
		}

		T& operator[](size_t _index)
		{
			return m_v[_index];
		}

		constexpr Vector3<T> operator+(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_v[0] + _other[0], m_v[1] + _other[1], m_v[2] + _other[2]);
		}

		constexpr Vector3<T> operator-(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_v[0] - _other[0], m_v[1] - _other[1], m_v[2] - _other[2]);
		}

		constexpr Vector3<T> operator*(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_v[0] * _other[0], m_v[1] * _other[1], m_v[2] * _other[2]);
		}

		constexpr Vector3<T> operator/(const Vector3<T>& _other) const
		{
			return Vector3<T>(m_v[0] / _other[0], m_v[1] / _other[1], m_v[2] / _other[2]);
		}

		constexpr Vector3<T>& operator+=(const Vector3<T>& _other)
		{
			m_v[0] += _other[0];
			m_v[1] += _other[1];
			m_v[2] += _other[2];
			return *this;
		}

		constexpr Vector3<T>& operator-=(const Vector3<T>& _other)
		{
			m_v[0] -= _other[0];
			m_v[1] -= _other[1];
			m_v[2] -= _other[2];
			return *this;
		}

		constexpr Vector3<T>& operator*=(const Vector3<T>& _other)
		{
			m_v[0] *= _other[0];
			m_v[1] *= _other[1];
			m_v[2] *= _other[2];
			return *this;
		}

		constexpr Vector3<T>& operator/=(const Vector3<T>& _other)
		{
			m_v[0] /= _other[0];
			m_v[1] /= _other[1];
			m_v[2] /= _other[2];
			return *this;
		}

		constexpr Vector3<T> operator+(T _scalar) const
		{
			return Vector3<T>(m_v[0] + _scalar, m_v[1] + _scalar, m_v[2] + _scalar);
		}

		constexpr Vector3<T> operator-(T _scalar) const
		{
			return Vector3<T>(m_v[0] - _scalar, m_v[1] - _scalar, m_v[2] - _scalar);
		}

		constexpr Vector3<T> operator*(T _scalar) const
		{
			return Vector3<T>(m_v[0] * _scalar, m_v[1] * _scalar, m_v[2] * _scalar);
		}

		constexpr Vector3<T> operator/(T _scalar) const
		{
			return Vector3<T>(m_v[0] / _scalar, m_v[1] / _scalar, m_v[2] / _scalar);
		}

		constexpr Vector3<T>& operator+=(T _scalar)
		{
			m_v[0] += _scalar;
			m_v[1] += _scalar;
			m_v[2] += _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator-=(T _scalar)
		{
			m_v[0] -= _scalar;
			m_v[1] -= _scalar;
			m_v[2] -= _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator*=(T _scalar)
		{
			m_v[0] *= _scalar;
			m_v[1] *= _scalar;
			m_v[2] *= _scalar;
			return *this;
		}

		constexpr Vector3<T>& operator/=(T _scalar)
		{
			m_v[0] /= _scalar;
			m_v[1] /= _scalar;
			m_v[2] /= _scalar;
			return *this;
		}

		constexpr bool operator==(const Vector3<T>& _other) const
		{
			return m_v[0] == _other[0] && m_v[1] == _other[1] && m_v[2] == _other[2];
		}

		constexpr bool operator!=(const Vector3<T>& _other) const
		{
			return m_v[0] != _other[0] || m_v[1] != _other[1] || m_v[2] != _other[2];
		}

		constexpr bool operator<(const Vector3<T>& _other) const
		{
			return m_v[0] < _other[0] && m_v[1] < _other[1] && m_v[2] < _other[2];
		}

		constexpr bool operator<=(const Vector3<T>& _other) const
		{
			return m_v[0] <= _other[0] && m_v[1] <= _other[1] && m_v[2] <= _other[2];
		}

		constexpr bool operator>(const Vector3<T>& _other) const
		{
			return m_v[0] > _other[0] && m_v[1] > _other[1] && m_v[2] > _other[2];
		}

		constexpr bool operator>=(const Vector3<T>& _other) const
		{
			return m_v[0] >= _other[0] && m_v[1] >= _other[1] && m_v[2] >= _other[2];
		}

	public:

		std::array<T, 3> m_v;
	};
}