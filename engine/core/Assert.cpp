#include "engine/engine_pch.hpp"
#include "Assert.hpp"

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

#include "engine/filesystem/Filesystem.hpp"
#include "engine/filesystem/FileHandle.hpp"
#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine
{
  namespace core
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

				std::cout << "Callstack:\n";

				HANDLE process = GetCurrentProcess();
				SymInitialize(process, NULL, TRUE);

				for (unsigned int i = 2; i < frames; i++)
				{
					SymFromAddr(process, (DWORD64)(callstack[i]), 0, symbol);
					std::cerr << i - 1 << ": " << symbol->Name << " - 0x" << symbol->Address << "\n";
				}
      }
#else
			const int MAX_CALLSTACK_DEPTH = 100;
			void* callstack[MAX_CALLSTACK_DEPTH];
			const int callstack_size = backtrace(callstack, MAX_CALLSTACK_DEPTH);
			char** callstack_symbols_strings = backtrace_symbols(callstack, callstack_size);

			std::cout << "Callstack:\n";
			for (auto i = 2; i < callstack_size; ++i)
			{
				std::cout << i - 1 << ": " << callstack_symbols_strings[i] << " - 0x" << callstack[i] << "\n";
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


      //std::optional<engine::fs::CFileHandle> assert_text_file = m_EngineFs->OpenFile("assertions.txt", engine::fs::EFileMode::ReadWrite | engine::fs::EFileMode::Append);

      //

      //assert_text_file->Close();

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    CAssertManager::CAssertManager()
    {
      //engine::events::CEventController::GetInstance().SubscribeEvent(CFi, this);
    }

    void CAssertManager::Assert(const std::string& _expression, const std::string& _message, const std::string& _asserting_filename, int _line_of_code)
    {
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

    void CAssertManager::OnEvent(engine::events::IEvent* _event)
    {

    }

    EAssertionAction CAssertManager::GetUserAction() const
    {
      std::cout << "Press 'i' to ignore, 'a' to abort, 'b' to break.\n";

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
}