#include "WeightTable.hpp"
#include "TableView.hpp"

#include <algorithm>
#include <random>

namespace pulvis::scriptable
{
	void CWeightTable::Add(const std::string& _value, float _weight)
	{
		if (_value.empty() || _weight <= 0.f)
		{
			return;
		}

		auto it = std::find_if(m_Entries.begin(), m_Entries.end(),
			[&](const SWeightEntry& _e) { return _e.Value == _value; });

		if (it != m_Entries.end())
		{
			it->Weight += _weight;
		}
		else
		{
			m_Entries.push_back({ _value, _weight });
		}
	}

	void CWeightTable::Load(const sol::table& _weight_table)
	{
		Clear();

		CTableView view(_weight_table);
		view.ForEachArrayItem([this](size_t /*_index*/, sol::object _value)
			{
				if (!_value.is<sol::table>())
				{
					return;
				}

				CTableView entry(_value.as<sol::table>());

				const std::string value = entry.ReadOr<std::string>("value", "");
				const float       weight = entry.ReadOr<float>("weight", 0.f);

				if (!value.empty() && weight > 0.f)
				{
					m_Entries.push_back({ value, weight });
				}
			});
	}

	void CWeightTable::Clear()
	{
		m_Entries.clear();
	}

	const std::string& CWeightTable::Select(uint64_t _seed) const
	{
		return SelectFrom(m_Entries, _seed);
	}

	const std::string& CWeightTable::SelectModified(uint64_t _seed, const std::vector<SWeightEntry>& _modifier) const
	{
		// Merge: modifier weights add on top of base; new keys are appended.
		std::vector<SWeightEntry> merged;
		merged.reserve(m_Entries.size() + _modifier.size());
		merged = m_Entries;

		for (const SWeightEntry& mod : _modifier)
		{
			if (mod.Value.empty())
			{
				continue;
			}

			auto it = std::find_if(merged.begin(), merged.end(),
				[&](const SWeightEntry& _e) { return _e.Value == mod.Value; });

			if (it != merged.end())
			{
				it->Weight = std::max(0.f, it->Weight + mod.Weight);
			}
			else if (mod.Weight > 0.f)
			{
				merged.push_back(mod);
			}
		}

		return SelectFrom(merged, _seed);
	}

	const std::string& CWeightTable::SelectFrom(const std::vector<SWeightEntry>& _entries, uint64_t _seed)
	{
		static const std::string s_Empty;

		if (_entries.empty())
		{
			return s_Empty;
		}

		double total = 0.0;
		for (const SWeightEntry& e : _entries)
		{
			if (e.Weight > 0.f)
			{
				total += static_cast<double>(e.Weight);
			}
		}

		if (total <= 0.0)
		{
			return _entries.front().Value;
		}

		std::mt19937_64 rng(_seed);
		std::uniform_real_distribution<double> dist(0.0, total);
		double pick = dist(rng);

		for (const SWeightEntry& e : _entries)
		{
			if (e.Weight <= 0.f)
			{
				continue;
			}
			pick -= static_cast<double>(e.Weight);
			if (pick <= 0.0)
			{
				return e.Value;
			}
		}

		return _entries.back().Value;
	}
}