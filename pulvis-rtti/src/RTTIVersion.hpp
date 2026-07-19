#pragma once

#include "DynamicLibraryExport.hpp"

namespace pulvis::rtti
{
	constexpr static short RTTI_ABI_VERSION = 1;

	PULVIS_DLL_API [[nodiscard]] short GetRttiAbiVersion();
}