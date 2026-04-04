#include "Assert.hpp"

#include "Logger.hpp"

#include <iostream>
#include <csignal>
#include <signal.h>

#ifdef WINDOWS_OS
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#else
#include <execinfo.h>
#endif

namespace pulvis::core
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////
		CAssertion::CAssertion(const std::string& _asserting_filename, int _line_of_code)
			: m_AssertingFilename(_asserting_filename)
			, m_CodeLine(_line_of_code)
		{
		}

		void CAssertion::PrintCallstack() const
		{
#ifdef WINDOWS_OS
			const int MAX_CALLSTACK_DEPTH = 64;
			void* callstack[MAX_CALLSTACK_DEPTH];
			unsigned short frames = CaptureStackBackTrace(0, MAX_CALLSTACK_DEPTH, callstack, nullptr);

			SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);

			if (symbol)
			{
				symbol->MaxNameLen = 255;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);


        PULVIS_INFO_LOG("[Assert] Callstack: ");

				HANDLE process = GetCurrentProcess();
				SymInitialize(process, NULL, TRUE);

				for (unsigned int i = 2; i < frames; i++)
				{
					SymFromAddr(process, (DWORD64)(callstack[i]), 0, symbol);
          PULVIS_INFO_LOG("{}: {}- 0x{}", i - 1, symbol->Name, symbol->Address);
				}
      }
#else
			const int MAX_CALLSTACK_DEPTH = 100;
			void* callstack[MAX_CALLSTACK_DEPTH];
			const int callstack_size = backtrace(callstack, MAX_CALLSTACK_DEPTH);
			char** callstack_symbols_strings = backtrace_symbols(callstack, callstack_size);

      PULVIS_INFO_LOG("[Assert] Callstack: ");
      for (auto i = 2; i < callstack_size; ++i)
			{
        PULVIS_INFO_LOG("{}: {}", callstack_symbols_strings[i], callstack[i]);
			}

			free(callstack_symbols_strings);
#endif
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    CAssertIgnoreList::CAssertIgnoreList()
    {
      // Initialize should load .txt file and fill vector.
    }

    bool CAssertIgnoreList::IsIgnored(const CAssertion& _assertion) const
    {
      for (const CAssertion& assert : m_AssertionIgnoreList)
			{
				if (assert.GetAssertingFilename() == _assertion.GetAssertingFilename() && assert.GetCodeLine() == _assertion.GetCodeLine())
				{
					return true;
				}
			}

      return false;
    }

    void CAssertIgnoreList::AddToIgnoreList(const CAssertion& _assertion)
    {
      m_AssertionIgnoreList.push_back(_assertion);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    CAssertionService::CAssertionService()
    {
#if defined(DEBUG)
			m_IsActive = true;
#else
			m_IsActive = false;
#endif
    }

    CAssertionService::~CAssertionService()
    {
    }

    void CAssertionService::Assert(const std::string& _expression, const std::string& _message, const std::string& _asserting_filename, int _line_of_code)
    {
      if (m_IsActive)
      {
        return;
      }

      CAssertion assertion(_asserting_filename, _line_of_code);

      if (m_IgnoreList.IsIgnored(assertion))
				return;

      std::cerr << "Assertion failed: " << _expression << "\n";
      std::cerr << "Message: " << _message << "\n";
      std::cerr << "File: " << _asserting_filename << "\n";
      std::cerr << "Line: " << _line_of_code << "\n";

      assertion.PrintCallstack();

      const EAssertionAction user_action = GetUserAction();

      switch (user_action)
      {
      case EAssertionAction::Ignore:
        m_IgnoreList.AddToIgnoreList(assertion);
        break;
      case EAssertionAction::Abort:
        std::raise(SIGABRT);
        break;
      case EAssertionAction::Debug:
#ifdef WINDOWS_OS
        __debugbreak();
#else
        std::raise(SIGTRAP);
#endif
        break;
      [[unlikely]] default: break;
      }
    }

    EAssertionAction CAssertionService::GetUserAction() const
    {
      PULVIS_INFO_LOG("Press 'i' to ignore, 'a' to abort, 'b' to break.");

      char input;
      std::cin >> input;

      switch (input)
      {
      case 'i': return EAssertionAction::Ignore;
      case 'a': return EAssertionAction::Abort;
      case 'b': return EAssertionAction::Debug;
      [[unlikely]] default:  return EAssertionAction::Abort;
      }
    }
}