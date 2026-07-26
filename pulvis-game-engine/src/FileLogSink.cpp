#include "FileLogSink.hpp"

#include <chrono>

namespace pulvis::game_engine
{
	CFileLogSink::CFileLogSink(pulvis::fs::CFileSystem& _file_system)
		: m_FileSystem(_file_system)
	{

	}

	CFileLogSink::~CFileLogSink()
	{
		Shutdown();
	}

	void CFileLogSink::Initialize(const pulvis::fs::CDomainRoots& _domains)
	{
		const auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
		const auto local_time = std::chrono::zoned_time(std::chrono::current_zone(), now);
		const std::string file_name = std::format("log_{:%Y%m%d_%H_%M_%S}.txt", local_time);
		const std::string file_path = std::format("user/logs/{}", file_name);
		m_LogFilePath = file_path;

		const std::string header = "LOGS\n";
		m_Buffer.Append(header.c_str(), header.size());

		if (m_FileSystem.WriteFile(pulvis::fs::EDomain::User, pulvis::fs::CFilePath(m_LogFilePath), m_Buffer) != pulvis::fs::EFileResult::Success)
		{
			PULVIS_ERROR_LOG("Failed to create log file: {}", m_LogFilePath);
			return;
		}

		m_SinkID = pulvis::core::CLogService::Get().RegisterSink(
			pulvis::tl::FastFunction::Make<void, const pulvis::core::SLogInfo&>(
				[this](const pulvis::core::SLogInfo& _log_info)
				{
					OnLog(_log_info);
				}),
			true /* dump_history */);

		ASSERT(m_SinkID != pulvis::core::INVALID_LOG_SINK_ID, "Sink registration failed");
	}

	void CFileLogSink::Shutdown()
	{
		if (m_SinkID != pulvis::core::INVALID_LOG_SINK_ID)
		{
			pulvis::core::CLogService::Get().UnregisterSink(m_SinkID);
			m_SinkID = pulvis::core::INVALID_LOG_SINK_ID;
		}

		if (!m_Buffer.Empty())
		{
			m_Buffer.Clear();
		}
	}

	void CFileLogSink::OnLog(const pulvis::core::SLogInfo& _log)
	{
		const std::string log_entry = _log.FileDump();
		m_Buffer.Append(log_entry.c_str(), log_entry.size());
		pulvis::fs::EFileResult result = m_FileSystem.WriteFile(pulvis::fs::EDomain::User, pulvis::fs::CFilePath(m_LogFilePath), m_Buffer);
		ASSERT(result == pulvis::fs::EFileResult::Success, "Failed to write log file");
	}
}