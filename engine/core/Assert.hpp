#pragma once

#include "engine/patterns/Singleton.hpp"

#define ASSERT(expr, message) \
  if (!(expr)) \
	{ \
		engine::core::AssertManager::GetInstance().Assert(#expr, message, __FILE__, __LINE__); \
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

    class Assertion
    {
		public:

			Assertion(const std::string& asserting_filename, int line_of_code);

			void PrintCallstack() const;

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

      bool IsIgnored(const Assertion& _assertion) const;
      void AddToIgnoreList(const Assertion& _assertion);

    private:

      std::vector<Assertion> m_AssertionIgnoreList;
    };

    class AssertManager : public Singleton<AssertManager>
    {
    public:

      void Assert(const std::string& _expression, const std::string& _message, const std::string& _asserting_filename, int _line_of_code);

    private:

      AssertManager() = default;
      ~AssertManager() = default;
      friend class Singleton<AssertManager>;

      EAssertionAction GetUserAction() const;

      AssertIgnoreList m_IgnoreList;
    };
  }
}