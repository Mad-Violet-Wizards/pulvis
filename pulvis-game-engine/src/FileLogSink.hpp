#pragma once

#include "Logger.hpp"
#include "FileSystem.hpp"
#include "FileBuffer.hpp"
#include "DomainRoots.hpp"

namespace pulvis::game_engine
{
	class CFileLogSink
	{
		public:

			CFileLogSink(pulvis::fs::CFileSystem& _file_system);
			~CFileLogSink();

			CFileLogSink(const CFileLogSink&) = delete;
			CFileLogSink& operator=(const CFileLogSink&) = delete;

			void Initialize(const pulvis::fs::CDomainRoots& _domains);
			void Shutdown();

		private:

			void OnLog(const pulvis::core::SLogInfo& _log);

		private:

			pulvis::fs::CFileBuffer m_Buffer;
			std::string m_LogFilePath;
			pulvis::fs::CFileSystem& m_FileSystem;

			pulvis::core::log_sink_id_t m_SinkID = pulvis::core::INVALID_LOG_SINK_ID;
	};
}