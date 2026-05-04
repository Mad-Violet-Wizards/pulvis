#pragma once

#include <sol3/sol.hpp>

#include <functional>
#include <string_view>

namespace pulvis::scriptable
{
	/*
		CTableView - ergonomic, type-safe wrapper over sol::table.

		Eliminates the need for hand-rolled lua_getfield / lua_pop / lua_isXxx
		boilerplate at every call site. Used by all systems that consume.

		Example:
			CTableView view(table);
			std::string name = view.ReadOr<std::string>("Name", "");
			int32_t sort     = view.ReadOr<int32_t>("SortKey", 0);
			view.ForEachArrayItem([](size_t _i, sol::object _v) { ... });
	*/
	class CTableView
	{
	public:

		explicit CTableView(sol::table _table) : m_Table(std::move(_table)) {}

		bool IsValid() const { return m_Table.valid(); }
		bool Has(std::string_view _key) const
		{
			if (!IsValid()) { return false; }
			sol::object o = m_Table[_key];
			return o.valid() && o.get_type() != sol::type::nil;
		}

		template<typename T>
		T ReadOr(std::string_view _key, const T& _default) const
		{
			if (!IsValid()) { return _default; }
			sol::optional<T> v = m_Table[_key];
			return v.value_or(_default);
		}

		template<typename T>
		sol::optional<T> TryRead(std::string_view _key) const
		{
			if (!IsValid()) { return sol::nullopt; }
			return m_Table[_key].template get<sol::optional<T>>();
		}

		CTableView Sub(std::string_view _key) const
		{
			if (!IsValid()) { return CTableView{ sol::table{} }; }
			sol::object o = m_Table[_key];
			return CTableView{ o.is<sol::table>() ? o.as<sol::table>() : sol::table{} };
		}

		// Iterates an array-like table (1..N). Callback receives (index, sol::object).
		void ForEachArrayItem(const std::function<void(size_t, sol::object)>& _cb) const
		{
			if (!IsValid()) { return; }
			const size_t count = m_Table.size();
			for (size_t i = 1; i <= count; ++i)
			{
				sol::object value = m_Table[i];
				if (value.valid())
				{
					_cb(i, value);
				}
			}
		}

		sol::table Raw() const { return m_Table; }

	private:

		sol::table m_Table;
	};
}