#pragma once

#include <sol3/sol.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace pulvis::scriptable
{
	struct SWeightEntry
	{
		std::string Value;
		float Weight = 0.f;
	};

	/*
		CWeightTable - deterministic weighted random selection.

		Backs the Lua-side `pulvis.weighted_select(table, seed)` helper used
		by worldgen scripts (planet types, civilization styles, biomes...).

		Loads its data from a sol::table of the form:
			{
				{ value = "Rocky",   weight = 1.0 },
				{ value = "Oceanic", weight = 0.8 },
				...
			}

		Selection is deterministic: same seed + same table = same result.
		SelectModified() allows per-call weight adjustments without mutating
		the base table (e.g. planet-type bias on a frozen world).
	*/
	class CWeightTable
	{
	public:

		void Add(const std::string& _value, float _weight);
		void Load(const sol::table& _weight_table);
		void Clear();

		const std::string& Select(uint64_t _seed) const;
		const std::string& SelectModified(uint64_t _seed, const std::vector<SWeightEntry>& _modifier) const;

		size_t Count() const { return m_Entries.size(); }
		bool   Empty() const { return m_Entries.empty(); }

		const std::vector<SWeightEntry>& GetEntries() const { return m_Entries; }

	private:

		static const std::string& SelectFrom(const std::vector<SWeightEntry>& _entries, uint64_t _seed);

		std::vector<SWeightEntry> m_Entries;
	};
}