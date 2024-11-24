#include "engine/engine_pch.hpp"
#include "MemoryProfiler.hpp"


namespace engine::memory
{
	//////////////////////////////////////////////////////////////////////////
	class CMemoryProfiler::Impl
	{
	public:

		bool m_IsActive = false;

		std::unordered_map<void*, EMemoryCategory> m_LookupTable;

		constexpr static size_t s_AllocRecordArrayCount = static_cast<size_t>(EMemoryCategory::Count);
		std::array<std::list<SAllocationRecord>, s_AllocRecordArrayCount> m_AllocationRecords;
	};

	CMemoryProfiler::CMemoryProfiler()
	{
		m_Impl = new Impl();
	}

	CMemoryProfiler::~CMemoryProfiler()
	{
		delete m_Impl;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMemoryProfiler::IncreaseMemoryUsage(const SAllocationRecord& _alloc_record)
	{
		m_Impl->m_AllocationRecords[static_cast<size_t>(_alloc_record.m_Category)].push_back(_alloc_record);
		m_Impl->m_LookupTable.insert({ _alloc_record.m_Address, _alloc_record.m_Category });
	}

	void CMemoryProfiler::DecreaseMemoryUsage(void* _ptr)
	{
		auto it = m_Impl->m_LookupTable.find(_ptr);
		if (it != m_Impl->m_LookupTable.end())
		{
			const size_t index = static_cast<size_t>(it->second);
			std::list<SAllocationRecord>& alloc_records = m_Impl->m_AllocationRecords[index];

			for (auto alloc_record_it = alloc_records.begin(); alloc_record_it != alloc_records.end();)
			{
				if (alloc_record_it->m_Address == _ptr)
				{
					alloc_records.erase(alloc_record_it);
					break;
				}
				else
				{
					++alloc_record_it;
				}
			}

			m_Impl->m_LookupTable.erase(it);
		}
	}

	size_t CMemoryProfiler::GetMemoryUsage(EMemoryCategory _mem_category) const
	{
		size_t sum = 0;

		const size_t index = static_cast<size_t>(_mem_category);
		const std::list<SAllocationRecord>& alloc_records = m_Impl->m_AllocationRecords[index];
		for (const auto& alloc_record : alloc_records)
		{
			sum += alloc_record.m_Size;
		}

		return sum;
	}

	void CMemoryProfiler::SetActive(bool _is_active)
	{
		m_Impl->m_IsActive = _is_active;
	}

	bool CMemoryProfiler::IsActive() const
	{
		return m_Impl->m_IsActive;
	}

	void CMemoryProfiler::DumpSummaryMemoryUsageConsole() const
	{
		for (size_t i = 0; i < static_cast<size_t>(EMemoryCategory::Count); ++i)
		{
			const EMemoryCategory mem_category = static_cast<EMemoryCategory>(i);
			const size_t memory_usage = GetMemoryUsage(mem_category);

			const std::string_view mem_category_text = engine::rtti::CRTTIEnum<EMemoryCategory>::ToString(mem_category);
			std::cout << "Memory usage for category " << mem_category_text << ": " << memory_usage << " bytes\n";
		}
	}

	void CMemoryProfiler::DumpFullMemoryUsageConsole() const
	{
		for (size_t i = 0; i < static_cast<size_t>(EMemoryCategory::Count); ++i)
		{
			const EMemoryCategory mem_category = static_cast<EMemoryCategory>(i);
			const size_t memory_usage = GetMemoryUsage(mem_category);

			const std::string_view mem_category_text = engine::rtti::CRTTIEnum<EMemoryCategory>::ToString(mem_category);
			std::cout << "Memory usage for category " << mem_category_text << ": " << memory_usage << " bytes\n";

			const std::list<SAllocationRecord>& alloc_records = m_Impl->m_AllocationRecords[i];
			for (const auto& alloc_record : alloc_records)
			{
				std::cout << "Address: " << alloc_record.m_Address << ", Size: " << alloc_record.m_Size << "\nCallstack:\n";
				for (const std::string& stacktrace : alloc_record.m_StackTrace)
				{
					std::cout << stacktrace << "\n";
				}
			}
		}
	}

	void CMemoryProfiler::DumpMemoryUsageFile() const
	{
	}
}
