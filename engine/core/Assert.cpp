#include "engine/engine_pch.hpp"
#include "Assert.hpp"

#include <iostream>

namespace core
{
///////////////////////////////////////////////////////////////////////////////////////////////////
IAssert::IAssert(const std::string& asserting_filename, int line_of_code)
    : m_AssertingFilename(asserting_filename)
    , m_CodeLine(line_of_code)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
AssertUnix::AssertUnix(const std::string& asserting_filename, int line_of_code)
    : IAssert(asserting_filename, line_of_code)
{

}

void AssertUnix::PrintCallstack() const
{
    std::cout << "[AssertUnix] PrintCallstack.\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AssertWindows::AssertWindows(const std::string& asserting_filename, int line_of_code)
    : IAssert(asserting_filename, line_of_code)
{

}

void AssertWindows::PrintCallstack() const
{
    std::cout << "[AssertWindows] PrintCallstack.\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AssertIgnoreList::AssertIgnoreList()
{
    // Initialize should load .txt file and fill vector.
}



}