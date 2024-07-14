#pragma once

namespace core
{
	class IAssert
  {
  	public:

      IAssert(const std::string& asserting_filename, int line_of_code);
      virtual ~IAssert() = default;

			virtual void PrintCallstack() const = 0;

		protected:

			std::string m_AssertingFilename;
			int m_CodeLine;
  };

  class AssertUnix : public IAssert
  {
		public:

			AssertUnix(const std::string& asserting_filename, int line_of_code);

			void PrintCallstack() const override;
  };

  class AssertWindows : public IAssert
  {
		public:
		
			AssertWindows(const std::string& asserting_filename, int line_of_code);

			void PrintCallstack() const override;
  };

  class AssertIgnoreList
  {
    public:

      AssertIgnoreList();

    private:

      std::vector<IAssert*> m_AssertionIgnoreList;
  };

  class AssertManager
  {
  };
}