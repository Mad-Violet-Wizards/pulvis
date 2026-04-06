#pragma once

#include "ChunkCoord.hpp"
#include "Logger.hpp"

#include <string>
#include <unordered_map>
#include <cstdint>

namespace pulvis::level
{
	/* 
		Region = abstract overlay on top of chunks.
		Defines properties of an area: biome, faction, resource chances, etc.
		One region may cover many chunks.

		Properties are stored as string -> variant map.
		This allows Lua scripts and editor to define arbitrary metadata.

		Example:

			region.Properties["faction"] = "empire"
			region.Properties["copper_chance"] = "0.3"
			region.Properties["danger_level"] = "5"
			region.Properties["ambient_shader"] = "shaders/dungeon_fog"
			region.Properties["music_track"] = "audio/music/dark_cave"
	*/

///////////////////////////////////////////////////////////////////////////////////////////////////
	struct SRegionProperties
	{
		std::unordered_map<std::string, std::string> Properties;

		void Set(const std::string& _key, const std::string& _value)
		{
			Properties[_key] = _value;
		}

		const std::string& GetString(const std::string& _key, const std::string& _default = "") const
		{
			auto it = Properties.find(_key);
			if (it != Properties.end())
			{
				return it->second;
			}
			return _default;
		}

		float GetFloat(const std::string& _key, float _default = 0.f) const
		{
			auto it = Properties.find(_key);
			if (it != Properties.end())
			{
				try
				{
					return std::stof(it->second);
				}
				catch (...)
				{
					PULVIS_ERROR_LOG("Failed to convert region property '{}' with value '{}' to float.", _key, it->second);
					return _default;
				}
			}
			return _default;
		}

		int GetInt(const std::string& _key, int _default = 0) const
		{
			auto it = Properties.find(_key);
			if (it != Properties.end())
			{
				try
				{
					return std::stoi(it->second);
				}
				catch (...)
				{
					PULVIS_ERROR_LOG("Failed to convert region property '{}' with value '{}' to int.", _key, it->second);
					return _default;
				}
			}
			return _default;
		}

		bool GetBool(const std::string& _key, bool _default = false) const
		{
			auto it = Properties.find(_key);
			if (it != Properties.end())
			{
				const std::string& value = it->second;
				if (value == "true" || value == "1")
					return true;
				if (value == "false" || value == "0")
					return false;
				PULVIS_ERROR_LOG("Failed to convert region property '{}' with value '{}' to bool. Expected 'true', 'false', '1', or '0'.", _key, value);
				return _default;
			}
			return _default;
		}

		bool Has(const std::string& _key) const
		{
			return Properties.find(_key) != Properties.end();
		}
	};

///////////////////////////////////////////////////////////////////////////////////////////////////
	struct SRegion
	{
		uint32_t RegionId = 0;
		std::string Name;
		SRegionProperties Properties;

		SChunkCoord Min;
		SChunkCoord Max;

		bool ContainsChunk(const SChunkCoord& _coord) const
		{
			return _coord.X >= Min.X && _coord.X <= Max.X && _coord.Y >= Min.Y && _coord.Y <= Max.Y;
		}
	};
}