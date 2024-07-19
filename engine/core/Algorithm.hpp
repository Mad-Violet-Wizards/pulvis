#pragma once

namespace engine
{
namespace core
{

	template<typename T>
	inline std::vector<T> Intersect(std::vector<T> _a, std::vector<T> _b)
	{
		std::vector<T> result;
		std::sort(_a.begin(), _a.end());
		std::sort(_b.begin(), _b.end());
		std::set_intersection(_a.begin(), _a.end(), _b.begin(), _b.end(), std::back_inserter(result));
		return result;
	}
}
}
