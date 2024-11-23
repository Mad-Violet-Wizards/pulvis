#include "engine/engine_pch.hpp"
#include "Logger.hpp"

#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine::core
{
	class CLogger::Impl
	{
		public:
			std::queue<SLogInfo> m_LogFileDumpQueue;

	};

	CLogger::CLogger()
	{
		m_Impl = new Impl();
	}

	CLogger::~CLogger()
	{
		delete m_Impl;
	}

	void CLogger::OnEvent(events::IEvent* _event)
	{
	}
	void CLogger::LogImpl(const SLogInfo& _log_info)
	{
		_log_info.ConsoleDump();
		m_Impl->m_LogFileDumpQueue.push(_log_info);
	}
}