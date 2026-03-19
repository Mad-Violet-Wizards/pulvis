#pragma once

#include <atomic>
#include <cstdint>

namespace pulvis::threads
{
	class CJobCounter
	{
		public:

			CJobCounter();

			explicit CJobCounter(int32_t _initial_count);

			CJobCounter(const CJobCounter& _other) = delete;
			CJobCounter& operator=(const CJobCounter& _other) = delete;

			void Increment(int32_t _value = 1);
			bool Decrement(int32_t _value = 1);

			int32_t Get() const;

			bool IsReady() const;

		public:

			void operator+=(int32_t _value);
			void operator-=(int32_t _value);

		private:

			std::atomic<int32_t> m_Count;
	};
}