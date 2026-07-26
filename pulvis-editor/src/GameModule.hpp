#pragma once

#include "DynamicLibrary.hpp"
#include "DomainRoots.hpp"

namespace pulvis::editor
{
	enum class EGameModuleState
	{
		NotLoaded,
		Loaded,
		FileNotFound,
		MissingAbiSymbol,
		AbiMismatch
	};

	class CGameModule
	{
		public:

			CGameModule() = default;
			~CGameModule();

			CGameModule(const CGameModule&) = delete;
			CGameModule& operator=(const CGameModule&) = delete;

			[[nodiscard]] EGameModuleState Load(const pulvis::fs::CDomainRoots& _domain_roots, pulvis::fs::EDomain _domain, std::string_view _module_name);
			void Unload();

			[[nodiscard]] bool IsLoaded() const noexcept;
			[[nodiscard]] EGameModuleState GetState() const noexcept;
			[[nodiscard]] short GetGameAbiVersion() const noexcept;

			template<typename Fn>
			[[nodiscard]] Fn GetSymbolAs(std::string_view _symbol_name) const noexcept
			{
				if (!m_Library.IsLoaded())
				{
					return nullptr;
				}

				return m_Library.GetSymbolAs<Fn>(_symbol_name);
			}

		private:

			pulvis::core::CDynamicLibrary m_Library;
			EGameModuleState m_State = EGameModuleState::NotLoaded;
			short m_GameAbiVersion = -1;

	};
}