#pragma once

#include <string>
#include <vector>

#ifdef DEBUG
#define ASSERT(expr, message) \
  if (!(expr)) \
	{ \
		pulvis::core::CAssertionService::GetInstance().Assert(#expr, message, __FILE__, __LINE__); \
	}
#else
#define ASSERT
#endif


namespace pulvis::core
{
 ///////////////////////////////////////////////////////////////////////////////////////////////////
    enum class EAssertionAction
    {
      Abort,
      Debug,
      Ignore
    };


///////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////
    class CAssertIgnoreList
    {
    public:

      CAssertIgnoreList();

      bool IsIgnored(const CAssertion& _assertion) const;
      void AddToIgnoreList(const CAssertion& _assertion);

    private:

      std::vector<CAssertion> m_AssertionIgnoreList;
    };

///////////////////////////////////////////////////////////////////////////////////////////////////
    class CAssertionService
    {
    public:

			~CAssertionService();

      void Assert(const std::string& _expression, const std::string& _message, const std::string& _asserting_filename, int _line_of_code);

      static CAssertionService& GetInstance();

    private:

      CAssertionService();

      EAssertionAction GetUserAction() const;

			CAssertIgnoreList m_IgnoreList;
			bool m_IsActive;
    };
  }