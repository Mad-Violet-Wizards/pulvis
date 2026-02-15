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

	CLogService& CLogService::GetInstance()
	{
		static CLogService log_service_instance;
		return log_service_instance;
	}

	void CLogService::LogImpl(const SLogInfo& _log_info)
	{
		std::cout << _log_info.ConsoleDump();
		m_LogFileDumpQueue.push(_log_info);
	}
}