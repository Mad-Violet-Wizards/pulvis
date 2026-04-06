#pragma once

#include "Rect.hpp"

namespace pulvis::tl::tests
{
	struct STestEntity
	{
		unsigned int ID;
		Rect<float> Bounds;
	};
}