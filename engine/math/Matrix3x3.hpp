#pragma once

#include "MathUtils.hpp"

namespace engine::math
{
	template<Arithmetic T>
	class Matrix3x3
	{
		private:
			Vector3<T> m_Matrix[3];
	};
}