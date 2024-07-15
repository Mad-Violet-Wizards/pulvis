#include "engine/engine_pch.hpp"
#include "Assert.hpp"

#include <iostream>
#include <csignal>
#include <signal.h>

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#endif
#include <execinfo.h>

namespace engine
{
  namespace core
  {
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    AssertUnix::AssertUnix(const std::string& asserting_filename, int line_of_code)
      : m_AssertingFilename(asserting_filename),
      m_CodeLine(line_of_code)
    {

    }

    void AssertUnix::PrintCallstack() const
    {
      const int MAX_CALLSTACK_DEPTH = 100;
      void* callstack[MAX_CALLSTACK_DEPTH];
      const int callstack_size = backtrace(callstack, MAX_CALLSTACK_DEPTH);
      char** callstack_symbols_strings = backtrace_symbols(callstack, callstack_size);

      std::cout << "Callstack:\n";
      for (auto i = 0; i < callstack_size; ++i)
      {
        std::cout << i << ": " << callstack_symbols_strings[i] << " - 0x" << callstack[i] << "\n";
      }

      free(callstack_symbols_strings);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    AssertWindows::AssertWindows(const std::string& asserting_filename, int line_of_code)
      : m_AssertingFilename(asserting_filename)
      , m_CodeLine(line_of_code)
    {

    }
#ifdef _WIN32
    void AssertWindows::PrintCallstack() const
    {
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

        for (unsigned int i = 0; i < frames; i++)
        {
          SymFromAddr(process, (DWORD64)(callstack[i]), 0, symbol);
          std::cerr << i << ": " << symbol->Name << " - 0x" << symbol->Address << "\n";
        }
      }
    }
#endif
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    AssertIgnoreList::AssertIgnoreList()
    {
      // Initialize should load .txt file and fill vector.
    }

    bool AssertIgnoreList::IsIgnored(const AssertWindows& assert_windows) const
    {
      return IsIgnoredInternal(assert_windows.GetAssertingFilename(), assert_windows.GetCodeLine());
    }

    bool AssertIgnoreList::IsIgnored(const AssertUnix& assert_unix) const
    {
      return IsIgnoredInternal(assert_unix.GetAssertingFilename(), assert_unix.GetCodeLine());
    }

#ifdef _WIN32
    void AssertIgnoreList::AddToIgnoreList(const AssertWindows& assert_windows)
    {
      m_AssertionIgnoreList.push_back(assert_windows);
    }
#else
    void AssertIgnoreList::AddToIgnoreList(const AssertUnix& assert_unix)
    {
      m_AssertionIgnoreList.push_back(assert_unix);
    }
#endif // _WIN32

    bool AssertIgnoreList::IsIgnoredInternal(const std::string& file_name, int line_of_code) const
    {
      for (const auto& assert : m_AssertionIgnoreList)
      {
        if (assert.GetAssertingFilename() == file_name && assert.GetCodeLine() == line_of_code)
        {
          return true;
        }
      }

      return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void AssertManager::Assert(const std::string& expression, const std::string& asserting_filename, int line_of_code)
    {
#ifdef _WIN32
      AssertWindows assertWindows(asserting_filename, line_of_code);

      if (m_IgnoreList.IsIgnored(assertWindows))
        return;
#else
      AssertUnix assertUnix(asserting_filename, line_of_code);

      if (m_IgnoreList.IsIgnored(assertUnix))
        return;
#endif

      std::cerr << "Assertion failed: " << expression << "\n";
      std::cerr << "File: " << asserting_filename << "\n";
      std::cerr << "Line: " << line_of_code << "\n";

#ifdef _WIN32
      assertWindows.PrintCallstack();
#else
      assertUnix.PrintCallstack();
#endif

      const EAssertionAction user_action = GetUserAction();

      switch (user_action)
      {
      case EAssertionAction::Ignore:
#ifdef _WIN32
        m_IgnoreList.AddToIgnoreList(assertWindows);
#else
        m_IgnoreList.AddToIgnoreList(assertUnix);
#endif
        break;
      case EAssertionAction::Abort:
        std::raise(SIGABRT);
        break;
      case EAssertionAction::Debug:
#ifdef _WIN32
        __debugbreak();
#else
        std::raise(SIGTRAP);
#endif
        break;
      [[unlikely]] default: break;
      }
    }

    EAssertionAction AssertManager::GetUserAction() const
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