#include "HotReloadWatcher.hpp"
#include "Logger.hpp"

namespace pulvis::scriptable
{
	void CHotReloadWatcher::Watch(const std::string& _relativeFilePath, const std::string& _absoluteFilePath, pulvis::tl::FastFunction _callback)
	{
		SWatchEntry entry;
		entry.RelativeFilePath = _relativeFilePath;
		entry.AbsoluteFilePath = _absoluteFilePath;
		entry.Callback = std::move(_callback);

		std::error_code ec;
		entry.LastWriteTime = std::filesystem::last_write_time(_absoluteFilePath, ec);

		if (ec)
		{
			PULVIS_ERROR_LOG("Failed to watch file '{}': {}", _relativeFilePath, ec.message());
			return;
		}

		m_WatchEntries[_relativeFilePath] = std::move(entry);
	}

	void CHotReloadWatcher::Unwatch(const std::string& _filePath)
	{
		m_WatchEntries.erase(_filePath);
	}

	void CHotReloadWatcher::Update(float _dt)
	{
		for (auto& [filePath, entry] : m_WatchEntries)
		{
			std::error_code ec;
			auto currentWriteTime = std::filesystem::last_write_time(entry.AbsoluteFilePath, ec);
			if (ec)
			{
				PULVIS_ERROR_LOG("Failed to check file '{}': {}", filePath, ec.message());
				continue;
			}
			if (currentWriteTime != entry.LastWriteTime)
			{
				entry.LastWriteTime = currentWriteTime;
				entry.Callback.Invoke<void>(entry.RelativeFilePath);
			}
		}
	}

	void CHotReloadWatcher::Clear()
	{
		m_WatchEntries.clear();
	}

}