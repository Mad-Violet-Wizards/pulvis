#pragma once

#include "MemoryCategory.hpp"



namespace engine::experimental::memory
{
	struct SAllocationRecord
	{
		void* m_Address;
		size_t m_Size;
		EMemoryCategory m_Category;
		std::vector<std::string> m_StackTrace;
	};

	class CMemoryProfiler : public Singleton<CMemoryProfiler>
	{
	public:

		CMemoryProfiler();
		~CMemoryProfiler();

		void IncreaseMemoryUsage(const SAllocationRecord& _alloc_record);
		void DecreaseMemoryUsage(void* _ptr);
		size_t GetMemoryUsage(EMemoryCategory _mem_category) const;

		void SetActive(bool _is_active);
		bool IsActive() const;

		void DumpSummaryMemoryUsageConsole() const;
		void DumpFullMemoryUsageConsole() const;
		void DumpMemoryUsageFile() const;

	private:

		friend class Singleton<CMemoryProfiler>;

		bool m_IsActive = false;

		std::unordered_map<void*, EMemoryCategory> m_LookupTable;

		constexpr static size_t s_AllocRecordArrayCount = static_cast<size_t>(EMemoryCategory::Count);
		std::array<std::list<SAllocationRecord>, s_AllocRecordArrayCount> m_AllocationRecords;

	};
}