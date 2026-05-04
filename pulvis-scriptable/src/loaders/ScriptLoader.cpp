#include "loaders/ScriptLoader.hpp"
#include "payloads/ScriptPayload.hpp"

#include "Logger.hpp"

namespace pulvis::scriptable
{
	bool CScriptLoader::Process(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("Script '{}': raw data empty.", _entry.VirtualPath);
			return false;
		}

		auto payload = std::make_unique<SScriptPayload>();
		payload->Source.assign(reinterpret_cast<const char*>(_entry.RawData.Data()), _entry.RawData.Size());
		_entry.Payload = std::move(payload);
		_entry.RawData.Clear();
		return true;
	}

	bool CScriptLoader::Unload(pulvis::fs::assets::SAssetEntry& _entry)
	{
		_entry.Payload = nullptr;
		return true;
	}
} // namespace pulvis::scriptable