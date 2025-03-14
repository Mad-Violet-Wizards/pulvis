#pragma once

namespace engine::rtti
{
	class CRTTITypeName
	{
		public:

	template<typename T>
	static constexpr std::string_view GetTypename() noexcept
	{
		constexpr std::string_view rawTypename = __FUNCSIG__;

		size_t start = 0;
		if constexpr (std::is_class<T>())
		{
			constexpr std::string_view to_trim_class { "GetTypename<class" };
			constexpr std::string_view to_trim_struct{ "GetTypename<struct" };

			start = rawTypename.find(to_trim_class);

			if (start == std::string::npos)
				start = rawTypename.find(to_trim_struct) + to_trim_struct.size() + 1;
			else
				start += to_trim_class.size() + 1;
			}
			else if constexpr (std::is_enum<T>())
			{
				constexpr std::string_view to_trim_enum{ "GetTypename<enum" };
				start = rawTypename.find(to_trim_enum) + to_trim_enum.size() + 1;
			}
			else
			{
				constexpr std::string_view to_trim_other { "GetTypename<" };
				start = rawTypename.find(to_trim_other) + to_trim_other.size();
			}

			const size_t end = rawTypename.find(">(void)") - start;
			return rawTypename.substr(start, end);
		}

	template<typename T>
	static constexpr std::string_view GetScopedTypename()
	{
		const std::string_view rawTypename = GetTypename<T>();
		const size_t start = rawTypename.find_last_of("::") + 1;
		const size_t end = rawTypename.size() - start;
		return rawTypename.substr(start, end);
	}
	};
}
