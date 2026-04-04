#pragma once

#include <cstdint>
#include <string>

#include "Assert.hpp"

namespace pulvis::fs
{
	using file_size_t = uint64_t;
	using file_offset_t = uint64_t;

	constexpr file_size_t INVALID_FILE_SIZE = UINT64_MAX;
//////////////////////////////////////////////////////////////////////////
	/* Outcome of all filesystem read/write/mount operations. */
	enum class EFileResult
	{
		Success,
		NotFound,
		AccessDenied,
		InvalidPath,
		IOError,
		NoSerializer,
		NoProcessor,
		DecompressionFailed,
		Cancelled,
		Unknown
	};

	/* Domain provides logical isolation between
	* engine resources, game assets, user data and dev tools.
	* 
	* Each domain mantains its own set of mount points with different priorities.
	* This prevents accidental cross-reads and allows clear separation of targets.
	* 
	* Engine - Engine-internal resources (shaders, default textures, engine config).
	* Game - Game assets (models, textures, levels).
	* User - per-user persistent data (%APPDATA%, saves, settings, cache, release dump).
	* Dev - developer tools and temporary files (logs, dumps, editor assets).
	*/

	enum class EDomain : uint8_t
	{
		Engine,
		Game,
		User,
		Dev,
		Count
	};

	constexpr unsigned int DOMAIN_COUNT = static_cast<unsigned int>(EDomain::Count);

	/* Determines resolution order when multiple sources are mounted
		 at the same virtual prefix within a domain. Higher tag value wins.*/
	enum class EMountTag : uint8_t
	{
		Base = 1,
		Game = 2,
		Patch = 3,
		Mod = 4
	};

	enum class EAssetType : uint16_t
	{
		Unknown,
		Texture,
		TextureDDS,
		Audio,
		Shader,
		Script,
		Font,
		Config,
		Level,
		Animation,
		Mesh,
		Material,
		Tileset,
		Generic
	};

	inline static EAssetType FromExtension(std::string_view _extension)
	{
		if (_extension == ".png" || _extension == ".jpg" || _extension == ".jpeg" || _extension == ".bmp" || _extension == ".tga")
		{
			return EAssetType::Texture;
		}
		else if (_extension == ".dds")
		{
			return EAssetType::TextureDDS;
		}
		else if (_extension == ".wav" || _extension == ".ogg" || _extension == ".mp3")
		{
			return EAssetType::Audio;
		}
		else if (_extension == ".vert" || _extension == ".frag" || _extension == ".geom" || _extension == ".glsl")
		{
			return EAssetType::Shader;
		}
		else if (_extension == ".lua")
		{
			return EAssetType::Script;
		}
		else if (_extension == ".ttf" || _extension == ".otf")
		{
			return EAssetType::Font;
		}
		else if (_extension == ".json" || _extension == ".xml" || _extension == ".ini")
		{
			return EAssetType::Config;
		}
		else if (_extension == ".lvl" || _extension == ".map")
		{
			return EAssetType::Level;
		}
		else if (_extension == ".anim" || _extension == ".aseprite")
		{
			return EAssetType::Animation;
		}
		else if (_extension == ".obj" || _extension == ".fbx" || _extension == ".gltf")
		{
			return EAssetType::Mesh;
		}
		else if (_extension == ".mat")
		{
			return EAssetType::Material;
		}
		else if (_extension == ".tileset")
		{
			return EAssetType::Tileset;
		}

		ASSERT(false, "Unknown mapping to asset type from extension: {} " + std::string(_extension));
		return EAssetType::Unknown;
	}

	enum class ECompressionType : uint8_t
	{
		None,
		Zlib,
		LZ4,
		Zstd
	};
}