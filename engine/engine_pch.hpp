#pragma once

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

// +STL
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <utility>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
#include <new>
#include <stack>
// -SLT

// +FMT
#include <fmt/format.h>
// -FMT

// +Engine
//	 +Core
#include "engine/core/Assert.hpp"
#include "engine/core/Logger.hpp"
#include "engine/core/UUID.hpp"
#include "engine/events/EventListener.hpp"
#include "engine/events/EventController.hpp"
#include "engine/memory/Memory.hpp"
//	-Core

//	+pstd
#include "engine/pstd/Intersects.hpp"
//	-pstd

//	+Math
#include "engine/math/Vector2.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/math/Vector4.hpp"


// -Engine
