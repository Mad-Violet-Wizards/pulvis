#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include <sol3/sol.hpp>

namespace pulvis::scriptable
{
	struct SWeightEntry
	{
		std::string Value;
		float Weight = 0.f;
	};

	class CWeightTable
	{
		public:

			void Add(const std::string& _value, float _weight);
			void Load(const sol::table& _weight_table);
			void Clear();

			const std::string& Select(uint64_t _seed) const;
			const std::string& SelectModified(uint64_t _seed, const std::vector<SWeightEntry>& _modifier) const;

			size_t Count() const;

		private:
			
			std::vector<SWeightEntry> m_Entries;
	};
}