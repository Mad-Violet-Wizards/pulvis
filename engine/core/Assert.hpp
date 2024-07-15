#pragma once

#include "engine/patterns/Singleton.hpp"

#define ASSERT(expr) \
  if (!(expr)) \
	{ \
		engine::core::AssertManager::GetInstance().Assert(#expr, __FILE__, __LINE__); \
	}

namespace engine
{
  namespace core
  {
    enum class EAssertionAction
    {
      Abort,
      Debug,
      Ignore
    };

    class AssertUnix
    {
    public:

      AssertUnix(const std::string& asserting_filename, int line_of_code);

      void PrintCallstack() const;

      const std::string& GetAssertingFilename() const { return m_AssertingFilename; }
      int GetCodeLine() const { return m_CodeLine; }

    private:

      std::string m_AssertingFilename;
      int m_CodeLine;
    };

    class AssertWindows
    {
    public:

      AssertWindows(const std::string& asserting_filename, int line_of_code);

#ifdef _WIN32
      void PrintCallstack() const;
#endif
      const std::string& GetAssertingFilename() const { return m_AssertingFilename; }
      int GetCodeLine() const { return m_CodeLine; }

    private:
      std::string m_AssertingFilename;
      int m_CodeLine;
    };

    class AssertIgnoreList
    {
    public:

      AssertIgnoreList();

      bool IsIgnored(const AssertWindows& assert_windows) const;
      bool IsIgnored(const AssertUnix& assert_unix) const;

#ifdef _WIN32
      void AddToIgnoreList(const AssertWindows& assert_windows);
#else
      void AddToIgnoreList(const AssertUnix& assert_unix);
#endif

    private:

      bool IsIgnoredInternal(const std::string& file_name, int line_of_code) const;

#ifdef _WIN32
      std::vector<AssertWindows> m_AssertionIgnoreList;
#else
      std::vector<AssertUnix> m_AssertionIgnoreList;
#endif
    };

    class AssertManager : public Singleton<AssertManager>
    {
    public:

      void Assert(const std::string& expression, const std::string& asserting_filename, int line_of_code);

    private:

      AssertManager() = default;
      ~AssertManager() = default;
      friend class Singleton<AssertManager>;

      EAssertionAction GetUserAction() const;

      AssertIgnoreList m_IgnoreList;
    };
  }
}