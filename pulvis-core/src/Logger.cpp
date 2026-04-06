#include "Logger.hpp"

#include <iostream>

namespace pulvis::core
{
	CLogService::CLogService()
	{
	}

	CLogService::~CLogService()
	{
	}

	void CLogService::LogImpl(const SLogInfo& _log_info)
	{
		std::printf("%s", _log_info.ConsoleDump().c_str());
		m_LogFileDumpQueue.push(_log_info);
	}
}