#pragma once

#include <type_traits>

#define STRINGIFY(x) #x
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
