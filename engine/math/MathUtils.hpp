#pragma once

#include <math.h>

namespace engine::math
{
	template<typename T>
	concept Arithmetic = requires(T a)
	{
		std::is_arithmetic_v<T>;
	};

	template<Arithmetic T>
	T Pow(T _val, unsigned int _power)
	{
		if (_power == 0)
			return 1;

		if (_power == 1)
			return _val;

		if (_val == 0)
			return 0;

		if (_val == 1)
			return 1;

		T final_val = 1;
		T temp_val = _val;
		while (_power > 0)
		{
			if (_power % 2 == 0)
			{
				_power /= 2;
				temp_val *= temp_val;
			}
			else
			{
				_power -= 1;
				final_val *= temp_val;
			}
		}

		return final_val;
	}

	template<Arithmetic T>
	T Abs(T _val)
	{
		return _val < 0 ? -_val : _val;
	}

	template<Arithmetic T>
	T Sqrt(T _val, [[maybe_unused]] int _precision = 25)
	{
		if (_val == 0)
			return 0;

		if (_val == 1)
			return 1;

		double final_val = _val / 2;
		while(_precision--)
		{
			final_val = (final_val + _val / final_val) / 2;
		}

		return static_cast<T>(final_val);
	}
}
