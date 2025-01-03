#include "engine/engine_pch.hpp"
#include "Memory.hpp"
#include "MemoryProfiler.hpp"

#ifdef WINDOWS_OS
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#endif

namespace engine::memory
{
	void* Allocate(EMemoryCategory _mem_category, size_t _size)
	{
		void* ptr = std::malloc(_size);
		
		if (CMemoryProfiler::GetInstance().IsActive())
		{
			SAllocationRecord alloc_record;
			alloc_record.m_Address = ptr;
			alloc_record.m_Size = _size;
			alloc_record.m_Category = _mem_category;
			alloc_record.m_StackTrace = GetStacktrace();

			CMemoryProfiler::GetInstance().IncreaseMemoryUsage(alloc_record);
		}

		return ptr;
	}

	void Deallocate(EMemoryCategory _mem_category, void* _ptr)
	{
		if (CMemoryProfiler::GetInstance().IsActive())
		{
			CMemoryProfiler::GetInstance().DecreaseMemoryUsage(_ptr);
		}

		std::free(_ptr);
	}

	void Deallocate(EMemoryCategory _mem_category, void* _ptr, size_t _size)
	{
		if (CMemoryProfiler::GetInstance().IsActive())
		{
			CMemoryProfiler::GetInstance().DecreaseMemoryUsage(_ptr);
		}



		std::free(_ptr);
	}

	std::vector<std::string> GetStacktrace()
	{
#ifdef WINDOWS_OS
	const HANDLE process = GetCurrentProcess();
	const HANDLE thread = GetCurrentThread();

	CONTEXT context;
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);

	STACKFRAME64 stack_frame;
	memset(&stack_frame, 0, sizeof(STACKFRAME64));
	stack_frame.AddrPC.Mode = AddrModeFlat;

	SymInitialize(process, NULL, TRUE);
	
#ifdef _M_X64
	const DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
#else
	const DWORD machineType = IMAGE_FILE_MACHINE_I386;
#endif

	std::vector<std::string> stacktrace;
	while (StackWalk64(machineType, process, thread, &stack_frame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
	{
		if (stack_frame.AddrPC.Offset == 0)
		{
			break;
		}

		char symbolBuffer[sizeof(SYMBOL_INFO) + 256];
		SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		if (SymFromAddr(process, stack_frame.AddrPC.Offset, 0, symbol))
		{
			const std::string symbol_name = symbol->Name;

			IMAGEHLP_LINE64 line;
			DWORD displacement;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			if (SymGetLineFromAddr64(process, stack_frame.AddrPC.Offset, &displacement, &line))
			{
				stacktrace.push_back(symbol_name + " - " + line.FileName + "(" + std::to_string(line.LineNumber) + ")");
			}
		}
	}

	return stacktrace;
#else
		// TODO.
#endif
	}

}