#pragma once

#include <cstddef>
#include <cstdint>

#if defined(WINDOWS_OS)
#include <Windows.h>
#endif

namespace pulvis::threads
{
	/*
		Utility to measure stack usage.

		GetStackUsed() -> cheap (no syscalls)
		QueryStackFromOS() -> uses NT API, heavier, but precise.

		Typical usage:
			auto [used, total] = StackUsage::QueryStackFromOS();
			printf("Stack: %.1f KB / %.1f KB\n", used / 1024.0, total / 1024.0);
	*/

	namespace StackUsage
	{
		inline const void* GetStackBase()
		{
			static volatile uint8_t anchor{ 0 };
			static const void* base = const_cast<const uint8_t*>(&anchor);
			return base;
		}

		inline size_t GetStackUsed()
		{
			volatile uint8_t here = 0;
			const auto* base = static_cast<const uint8_t*>(GetStackBase());
			const auto* now = const_cast<const uint8_t*>(&here);

			// Stack grows downward on x86/x64.
			return static_cast<size_t>(base - now);
		}

#if defined(WINDOWS_OS)
		struct SStackInfo
		{
			size_t Used;
			size_t Committed;
			size_t Reserved; // total stack size
		};

		inline SStackInfo QueryStackFromOS()
		{
			ULONG_PTR low = 0;
			ULONG_PTR high = 0;
			GetCurrentThreadStackLimits(&low, &high);

			volatile uint8_t here = 0;
			const auto sp = reinterpret_cast<uintptr_t>(const_cast<const uint8_t*>(&here));

			return SStackInfo{
				static_cast<size_t>(high - sp),		// used
				static_cast<size_t>(sp - low),		// committed (remaining)
				static_cast<size_t>(high - low)		// total reserved
			};
		}
#endif
	}
}