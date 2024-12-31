#pragma once

namespace engine::memory
{
	class PULVIS_API IAllocator
	{
		public:

			virtual ~IAllocator() noexcept = default;

			virtual void Reset() noexcept = 0;
			virtual void Release() noexcept = 0;
			virtual void ZeroMemory() noexcept = 0;
	};
}