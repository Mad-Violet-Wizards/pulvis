#include "ShaderLoader.hpp"
#include "assets/AssetLoader.hpp"
#include "assets/payloads/ShaderPayload.hpp"
#include "Logger.hpp"

#include <memory>

namespace pulvis::rendering
{
	bool CShaderLoader::Process(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("Shader asset '{}' has empty raw data.", _entry.VirtualPath);
			return false;
		}

		std::string_view shader_code = _entry.RawData.ToStringView();
		std::unique_ptr<pulvis::fs::assets::SShaderPayload> payload = std::make_unique<pulvis::fs::assets::SShaderPayload>();

		_entry.Payload = std::move(payload);
		return true;
	}

	bool CShaderLoader::Unload(pulvis::fs::assets::SAssetEntry& _entry)
	{
		_entry.Payload.reset();
		_entry.RawData.Clear();
		return true;
	}
}