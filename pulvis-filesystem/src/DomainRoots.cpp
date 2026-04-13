#include "DomainRoots.hpp"

#include "Assert.hpp"
#include "Logger.hpp"

#include <cstdlib>

namespace pulvis::fs
{
	CDomainRoots::CDomainRoots(const std::string& _app_name, const std::string& _game_assets_path)
	{
		m_Roots[static_cast<uint8_t>(EDomain::User)] = ResolveUserDataPath(_app_name);


#if defined(DEBUG)
		const std::string game_assets_dir = _game_assets_path.empty()
			? std::format("{}-assets", _app_name)
			: _game_assets_path;


		const std::filesystem::path engine_workspace = std::filesystem::current_path().parent_path();
		const std::filesystem::path game_workspace = std::filesystem::current_path();

		m_Roots[static_cast<uint8_t>(EDomain::Engine)] = engine_workspace / "pulvis-assets/engine/";
		m_Roots[static_cast<uint8_t>(EDomain::Game)] = game_workspace / game_assets_dir / "game/";
		m_Roots[static_cast<uint8_t>(EDomain::Dev)] = game_workspace / game_assets_dir / "dev/";

		PULVIS_INFO_LOG("Domain roots initialized:");
		PULVIS_INFO_LOG("Engine: {}", m_Roots[static_cast<uint8_t>(EDomain::Engine)].string());
		PULVIS_INFO_LOG("Game: {}", m_Roots[static_cast<uint8_t>(EDomain::Game)].string());
		PULVIS_INFO_LOG("Dev: {}", m_Roots[static_cast<uint8_t>(EDomain::Dev)].string());
		PULVIS_INFO_LOG("User: {}", m_Roots[static_cast<uint8_t>(EDomain::User)].string());
#elif defined(RELEASE)
		const std::filesystem::path exe_dir = std::filesystem::current_path();

		m_Roots[static_cast<uint8_t>(EDomain::Engine)] = exe_dir / "assets/engine/";
		m_Roots[static_cast<uint8_t>(EDomain::Game)] = exe_dir / "assets/game/";
#endif
	}

	const std::filesystem::path& CDomainRoots::GetRoot(EDomain _domain) const
	{
		ASSERT(_domain != EDomain::Count, "Invalid domain");
		return m_Roots[static_cast<uint8_t>(_domain)];
	}

	bool CDomainRoots::IsValid(EDomain _domain) const
	{
		ASSERT(_domain != EDomain::Count, "Invalid domain");
		const std::filesystem::path& root = m_Roots[static_cast<uint8_t>(_domain)];
		return !root.empty() && std::filesystem::exists(root);
	}

	void CDomainRoots::OverrideRoot(EDomain _domain, std::filesystem::path _new_root)
	{
		ASSERT(_domain != EDomain::Count, "Invalid domain.");
		m_Roots[static_cast<uint8_t>(_domain)] = std::move(_new_root);
	}

	std::filesystem::path CDomainRoots::ResolveUserDataPath(const std::string& _app_name)
	{
#if defined(WINDOWS_OS)
		char* appdata_buffer = nullptr;
		size_t buffer_size = 0;

		const errno_t error = _dupenv_s(&appdata_buffer, &buffer_size, "APPDATA");

		if (error != 0 || appdata_buffer == nullptr || buffer_size == 0)
		{
			if (appdata_buffer)
			{
				free(appdata_buffer);
			}

			ASSERT(false, "Failed to resolve user data path from environment variable APPDATA.");
			return {};
		}

		std::filesystem::path user_data_path(appdata_buffer);
		free(appdata_buffer);
		return user_data_path / _app_name;
#elif defined(MAC_OS)
		const char* home = std::getenv("HOME");
		
		if (home == nullptr)
		{
			ASSERT(false, "Failed to resolve user data path from environment variable HOME.");
			return {};
		}

		std::filesystem::path user_data_path(home);
		return user_data_path / "Library" / "Application Support" / _app_name;
#endif

		return {};
	}
}