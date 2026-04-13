#pragma once

#include "FastFunction.hpp"

#include <string>
#include <unordered_map>
#include <filesystem>

namespace pulvis::scriptable
{
	class CHotReloadWatcher
	{
		public:

			void Watch(const std::string& _relativeFilePath, const std::string& _absoluteFilePath, pulvis::tl::FastFunction _callback);
			void Unwatch(const std::string& _filePath);
			void Update(float _dt);

			void Clear();

		private:

			struct SWatchEntry
			{
				std::string RelativeFilePath;
				std::string AbsoluteFilePath;
				std::filesystem::file_time_type LastWriteTime;
				pulvis::tl::FastFunction Callback;
			};

			std::unordered_map<std::string, SWatchEntry> m_WatchEntries;
	};
}