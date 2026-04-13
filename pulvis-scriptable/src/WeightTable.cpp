#include "WeightTable.hpp"
#include "math/SeedMath.hpp"
#include "Assert.hpp"

namespace pulvis::scriptable
{
	static const std::string EMPTY_STRING;

	void CWeightTable::Add(const std::string& _value, float _weight)
	{
		m_Entries.push_back({ _value, _weight });
	}

	void CWeightTable::Clear()
	{
		m_Entries.clear();
	}

	void CWeightTable::Load(const sol::table& _weight_table)
	{
		Clear();

		for (size_t i = 1; i <= _weight_table.size(); ++i)
		{
			sol::optional<sol::table> row = _weight_table[i];

			if (!row.has_value())
			{
				continue;
			}

			sol::table row_table = row.value();
			sol::optional<std::string> value = row_table["value"];
			sol::optional<float> weight = row_table["weight"];

			if (!value.has_value())
			{
				ASSERT(false, "Weight table entry is missing a value");
				continue;
			}

			if (!weight.has_value())
			{
				ASSERT(false, "Weight table entry is missing a weight");
				continue;
			}

			Add(value.value(), weight.value());
		}
	}

	const std::string& CWeightTable::Select(uint64_t _seed) const
	{
		if (m_Entries.empty())
		{
			return EMPTY_STRING;
		}

		float total = 0.f;
		for (const SWeightEntry& entry : m_Entries)
		{
			total += entry.Weight;
		}

		if (total <= 0.f)
		{
			ASSERT(false, "Total weight is zero or negative");
			return EMPTY_STRING;
		}

		const float roll = pulvis::core::math::NormalizedFloat(_seed) * total;
		float cumulative = 0.f;

		for (const SWeightEntry& entry : m_Entries)
		{
			cumulative += entry.Weight;
			if (roll <= cumulative)
			{
				return entry.Value;
			}
		}

		return m_Entries.back().Value;
	}

	const std::string& CWeightTable::SelectModified(uint64_t _seed, const std::vector<SWeightEntry>& _modifier) const
	{
		return EMPTY_STRING;
	}
}