#pragma once

#include <string_view>

namespace pulvis::core
{
	class CDynamicLibrary
	{
		public:

			CDynamicLibrary();
			explicit CDynamicLibrary(std::string_view _path);
			~CDynamicLibrary();

			CDynamicLibrary(const CDynamicLibrary&) = delete;
			CDynamicLibrary& operator=(const CDynamicLibrary&) = delete;

			CDynamicLibrary(CDynamicLibrary&& _other) noexcept;
			CDynamicLibrary& operator=(CDynamicLibrary&& _other) noexcept;

			[[nodiscard]] bool IsLoaded() const noexcept;
			[[nodiscard]] void* GetSymbol(std::string_view _symbol_name) const noexcept;

			template<typename Fn>
			[[nodiscard]] Fn GetSymbolAs(std::string_view _symbol_name) const noexcept
			{
				return reinterpret_cast<Fn>(GetSymbol(_symbol_name));
			}

			void Unload() noexcept;

		private:

			void* m_Handle;
	};
}