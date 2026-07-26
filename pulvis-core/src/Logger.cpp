#include "Logger.hpp"

#include <iostream>

namespace pulvis::core
{
	CLogService::CLogService()
		: m_CacheCount(0)
	{
	}

	CLogService::~CLogService()
	{
	}

	void CLogService::LogImpl(const SLogInfo& _log_info)
	{
		std::printf("%s", _log_info.ConsoleDump().c_str());

		std::lock_guard lock(m_SinksMutex);
		for (SSinkEntry& sink_entry : m_Sinks)
		{
			sink_entry.Sink.Invoke<void>(_log_info);
		}

		CacheLog(_log_info);
	}

	void CLogService::CacheLog(SLogInfo _log_info)
	{
		m_CacheCount++;

		if (m_CacheCount >= CACHE_MAX_LOGS)
		{
			std::fill_n(m_LogCache.begin(), CACHE_MAX_LOGS, SLogInfo());
			m_CacheCount = 0;
		}

		m_LogCache[m_CacheCount] = _log_info;
	}

	log_sink_id_t CLogService::RegisterSink(pulvis::tl::FastFunction _sink, bool dump_history /* = false*/)
	{
		if (!_sink)
		{
			return INVALID_LOG_SINK_ID;
		}

		std::lock_guard lock(m_SinksMutex);
		m_Sinks.push_back({ m_NextSinkID, std::move(_sink) });

		if (dump_history)
		{
			pulvis::tl::FastFunction &sink = m_Sinks.back().Sink;
			for (size_t i = 0; i < m_CacheCount; ++i)
			{
				sink.Invoke<void>(m_LogCache[i]);
			}
		}

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