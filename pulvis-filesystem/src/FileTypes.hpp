#pragma once

#include <cstdint>
#include <limits>

namespace pulvis::fs
{
	using file_size_t = uint64_t;
	using file_offset_t = uint64_t;

	constexpr file_size_t INVALID_FILE_SIZE = std::numeric_limits<uint64_t>::max();

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
		Generic
	};

	enum class ECompressionType : uint8_t
	{
		None,
		Zlib,
		LZ4,
		Zstd
	};
}