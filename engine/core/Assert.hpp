#pragma once

#include "engine/core/Export.hpp"
#include "engine/patterns/Singleton.hpp"

#define ASSERT(expr, message) \
  if (!(expr)) \
	{ \
		engine::core::CAssertManager::GetInstance().Assert(#expr, message, __FILE__, __LINE__); \
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

    class CAssertion
    {
		public:

			CAssertion(const std::string& asserting_filename, int line_of_code);

			void PrintCallstack() const;

			const std::string& GetAssertingFilename() const { return m_AssertingFilename; }
			int GetCodeLine() const { return m_CodeLine; }

		private:

			std::string m_AssertingFilename;
			int m_CodeLine;
    };

    class CAssertIgnoreList
    {
    public:

      CAssertIgnoreList();

      bool IsIgnored(const CAssertion& _assertion) const;
      void AddToIgnoreList(const CAssertion& _assertion);

    private:

      std::vector<CAssertion> m_AssertionIgnoreList;
    };

    class PULVIS_API CAssertManager : public Singleton<CAssertManager>
    {
    public:

      void Assert(const std::string& _expression, const std::string& _message, const std::string& _asserting_filename, int _line_of_code);

    private:

      CAssertManager() = default;
      ~CAssertManager() = default;
      friend class Singleton<CAssertManager>;

      EAssertionAction GetUserAction() const;

      CAssertIgnoreList m_IgnoreList;
    };
  }
}