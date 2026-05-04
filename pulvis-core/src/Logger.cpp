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

		std::lock_guard lock(m_SinksMutex);
		for (SSinkEntry& sink_entry : m_Sinks)
		{
			sink_entry.Sink.Invoke<void>(_log_info);
		}
	}

	log_sink_id_t CLogService::RegisterSink(pulvis::tl::FastFunction _sink)
	{
		if (!_sink)
		{
			return INVALID_LOG_SINK_ID;
		}

		std::lock_guard lock(m_SinksMutex);
		m_Sinks.push_back({ m_NextSinkID, std::move(_sink) });
		return m_NextSinkID++;
	}

	bool CLogService::UnregisterSink(log_sink_id_t _sink_id)
	{
		std::lock_guard lock(m_SinksMutex);
		auto it = std::remove_if(m_Sinks.begin(), m_Sinks.end(), [_sink_id](const SSinkEntry& entry) { return entry.ID == _sink_id; });
		if (it != m_Sinks.end())
		{
			m_Sinks.erase(it, m_Sinks.end());
			return true;
		}
		return false;
	}
}