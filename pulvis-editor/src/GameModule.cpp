#include "GameModule.hpp"
#include "RTTIVersion.hpp"
#include "Logger.hpp"
#include <filesystem>

namespace pulvis::editor
{
	CGameModule::~CGameModule()
	{
		Unload();
	}


	EGameModuleState CGameModule::Load(const pulvis::fs::CDomainRoots& _domain_roots, pulvis::fs::EDomain _domain, std::string_view _module_name)
	{
		Unload();

		const std::filesystem::path module_path = _domain_roots.GetRoot(_domain) / _module_name;

		if (!std::filesystem::exists(module_path))
		{
			PULVIS_ERROR_LOG("Game module not found: {}", module_path.string());
			m_State = EGameModuleState::FileNotFound;
			return m_State;
		}

		m_Library = pulvis::core::CDynamicLibrary(module_path.string());

		if (!m_Library.IsLoaded())
		{
			PULVIS_ERROR_LOG("Failed to load game module: {}", module_path.string());
			m_State = EGameModuleState::FileNotFound;
			return m_State;
		}

		const short game_abi_version = m_Library.GetSymbolAs<short(*)()>("Game_GetRttiAbiVersion")();
		if (game_abi_version != pulvis::rtti::GetRttiAbiVersion())
		{
			PULVIS_ERROR_LOG("Game module ABI version mismatch: {} (expected {})", game_abi_version, pulvis::rtti::GetRttiAbiVersion());
			m_State = EGameModuleState::AbiMismatch;
			return m_State;
		}

		PULVIS_INFO_LOG("Game module loaded: {} (ABI version {})", module_path.string(), game_abi_version);
		m_State = EGameModuleState::Loaded;
		m_GameAbiVersion = game_abi_version;
		return m_State;
	}

	void CGameModule::Unload()
	{
		m_Library.Unload();
		m_State = EGameModuleState::NotLoaded;
		m_GameAbiVersion = -1;
	}

	bool CGameModule::IsLoaded() const noexcept
	{
		return m_State == EGameModuleState::Loaded;
	}

	EGameModuleState CGameModule::GetState() const noexcept
	{
		return m_State;
	}

	short CGameModule::GetGameAbiVersion() const noexcept
	{
		return m_GameAbiVersion;
	}


}