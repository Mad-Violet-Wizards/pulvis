#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Vector4
	{
		public:

		constexpr Vector4()
		{
			m_v[0] = 0;
			m_v[1] = 0;
			m_v[2] = 0;
			m_v[3] = 0;
		}

		constexpr Vector4(T _x, T _y, T _z, T _a)
		{
			m_v[0] = _x;
			m_v[1] = _y;
			m_v[2] = _z;
			m_v[3] = _a;
		}

		Vector4(const Vector4<T>& _other) = default;
		Vector4(Vector4<T>&& _other) noexcept = default;
		Vector4<T>& operator= (const Vector4<T>& _other) = default;
		Vector4<T>& operator= (Vector4<T>&& _other) noexcept = default;

		template<typename U>
		constexpr Vector4<U> Cast() const
		{
			return Vector4<U>(static_cast<U>(m_v[0]), static_cast<U>(m_v[1]), static_cast<U>(m_v[2]), static_cast<U>(m_v[3]));
		}

		T Length() const
		{
			return std::sqrt(std::pow(m_v[0], 2) + std::pow(m_v[1], 2) + std::pow(m_v[2], 2) + std::pow(m_v[3], 2));
		}

		T LengthSquared() const
		{
			return std::pow(m_v[0], 2) + std::pow(m_v[1], 2) + std::pow(m_v[2], 2) + std::pow(m_v[3], 2);
		}

		T Dot(const Vector4<T>& _other) const
		{
			return m_v[0] * _other[0] + m_v[1] * _other[1] + m_v[2] * _other[2] + m_v[3] * _other[3];
		}

		Vector4<T> Cross(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_v[1] * _other[2] - m_v[2] * _other[1],
							   m_v[2] * _other[0] - m_v[0] * _other[2],
							   m_v[0] * _other[1] - m_v[1] * _other[0],
							   0 };
		}

		Vector4<T> Normalize() const
		{
			const T length = Length();

			if (length == 0)
				return Vector4<T>{ 0, 0, 0, 0 };

			return Vector4<T>{ m_v[0] / length, m_v[1] / length, m_v[2] / length, m_v[3] / length };
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

		constexpr Vector4<T> operator+(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_v[0] + _other[0], m_v[1] + _other[1], m_v[2] + _other[2], m_v[3] + _other[3]};
		}

		constexpr Vector4<T> operator-(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_v[0] - _other[0], m_v[1] - _other[1], m_v[2] - _other[2], m_v[3] - _other[3] };
		}

		constexpr Vector4<T> operator*(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_v[0] * _other[0], m_v[1] * _other[1], m_v[2] * _other[2], m_v[3] * _other[3] };
		}

		constexpr Vector4<T> operator/(const Vector4<T>& _other) const
		{
			return Vector4<T>{ m_v[0] / _other[0], m_v[1] / _other[1], m_v[2] / _other[2], m_v[3] / _other[3] };
		}

		constexpr Vector4<T>& operator+=(const Vector4<T>& _other)
		{
			m_v[0] += _other[0];
			m_v[1] += _other[1];
			m_v[2] += _other[2];
			m_v[3] += _other[3];
			return *this;
		}

		constexpr Vector4<T>& operator-=(const Vector4<T>& _other)
		{
			m_v[0] -= _other[0];
			m_v[1] -= _other[1];
			m_v[2] -= _other[2];
			m_v[3] -= _other[3];
			return *this;
		}

		constexpr Vector4<T>& operator*=(const Vector4<T>& _other)
		{
			m_v[0] *= _other[0];
			m_v[1] *= _other[1];
			m_v[2] *= _other[2];
			m_v[3] *= _other[3];
			return *this;
		}

		constexpr Vector4<T>& operator/=(const Vector4<T>& _other)
		{
			m_v[0] /= _other[0];
			m_v[1] /= _other[1];
			m_v[2] /= _other[2];
			m_v[3] /= _other[3];
			return *this;
		}

		constexpr Vector4<T> operator+(T _scalar) const
		{
			return Vector4<T>{ m_v[0] + _scalar, m_v[1] + _scalar, m_v[2] + _scalar, m_v[3] + _scalar };
		}

		constexpr Vector4<T> operator-(T _scalar) const
		{
			return Vector4<T>(m_v[0] - _scalar, m_v[1] - _scalar, m_v[2] - _scalar, m_v[3] - _scalar);
		}

		constexpr Vector4<T> operator*(T _scalar) const
		{
			return Vector4<T> { m_v[0] * _scalar, m_v[1] * _scalar, m_v[2] * _scalar, m_v[3] * _scalar };
		}

		constexpr Vector4<T> operator/(T _scalar) const
		{
			return Vector4<T>{ m_v[0] / _scalar, m_v[1] / _scalar, m_v[2] / _scalar, m_v[3] / _scalar };
		}

		constexpr Vector4<T>& operator+=(T _scalar)
		{
			m_v[0] += _scalar;
			m_v[1] += _scalar;
			m_v[2] += _scalar;
			m_v[3] += _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator-=(T _scalar)
		{
			m_v[0] -= _scalar;
			m_v[1] -= _scalar;
			m_v[2] -= _scalar;
			m_v[3] -= _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator*=(T _scalar)
		{
			m_v[0] *= _scalar;
			m_v[1] *= _scalar;
			m_v[2] *= _scalar;
			m_v[3] *= _scalar;
			return *this;
		}

		constexpr Vector4<T>& operator/=(T _scalar)
		{
			m_v[0] /= _scalar;
			m_v[1] /= _scalar;
			m_v[2] /= _scalar;
			m_v[3] /= _scalar;
			return *this;
		}

		constexpr bool operator==(const Vector4<T>& _other) const
		{
			return m_v[0] == _other[0] && m_v[1] == _other[1] && m_v[2] == _other[2] && m_v[3] == _other[3];
		}

		constexpr bool operator!=(const Vector4<T>& _other) const
		{
			return m_v[0] != _other[0] || m_v[1] != _other[1] || m_v[2] != _other[2] || m_v[3] != _other[3];
		}

		constexpr bool operator<(const Vector4<T>& _other) const
		{
			return m_v[0] < _other[0] && m_v[1] < _other[1] && m_v[2] < _other[2] && m_v[3] < _other[3];
		}

		constexpr bool operator<=(const Vector4<T>& _other) const
		{
			return m_v[0] <= _other[0] && m_v[1] <= _other[1] && m_v[2] <= _other[2] && m_v[3] <= _other[3];
		}

		constexpr bool operator>(const Vector4<T>& _other) const
		{
			return m_v[0] > _other[0] && m_v[1] > _other[1] && m_v[2] > _other[2] && m_v[3] > _other[3];
		}

		constexpr bool operator>=(const Vector4<T>& _other) const
		{
			return m_v[0] >= _other[0] && m_v[1] >= _other[1] && m_v[2] >= _other[2] && m_v[3] >= _other[3];
		}

	public:

		std::array<T, 4> m_v;
	};
}