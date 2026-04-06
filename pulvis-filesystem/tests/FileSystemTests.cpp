#include "FileSystem.hpp"
#include "FileSourceMemory.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;

//////////////////////////////////////////////////////////////////////////
namespace
{
	std::shared_ptr<CFileSourceMemory> CreateMemorySource(std::initializer_list<std::pair<const char*, const char*>> _files)
	{
		auto source = std::make_shared<CFileSourceMemory>();
		for (const auto& [path, content] : _files)
		{
			source->Store(CFilePath(path), CFileBuffer(content, std::strlen(content)));
		}
		return source;
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (initialize & shutdown)", "[FS][FileSystem]")
{
	SECTION("Initialize and Shutdown do not throw")
	{
		Initialize();
		Shutdown();
	}

	SECTION("Double Initialize and Shutdown is safe")
	{
		Initialize();
		Initialize();
		Shutdown();
		Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (mount & unmount)", "[FS][FileSystem]")
{
	SECTION("Mount adds to MountSystem")
	{
		auto source = CreateMemorySource({ {"hero.lua", "print('hello')"} });
		Mount(EDomain::Game, CFilePath("scripts"), source);

		CHECK(GetMountSystem().HasMount(EDomain::Game, CFilePath("scripts")));
		CHECK(GetMountSystem().MountCount(EDomain::Game) == 1);

		Unmount(EDomain::Game, CFilePath("scripts"));
	}

	SECTION("Unmount removes from MountSystem")
	{
		auto source = CreateMemorySource({ {"data.txt", "content"} });
		Mount(EDomain::Game, CFilePath("assets"), source);
		Unmount(EDomain::Game, CFilePath("assets"));

		CHECK(!GetMountSystem().HasMount(EDomain::Game, CFilePath("assets")));
		CHECK(GetMountSystem().MountCount(EDomain::Game) == 0);
	}

	SECTION("Mount with custom tag")
	{
		auto source = CreateMemorySource({ {"mod.lua", "modded"} });
		Mount(EDomain::Game, CFilePath("mods"), source, EMountTag::Mod);

		CHECK(GetMountSystem().HasMount(EDomain::Game, CFilePath("mods")));

		Unmount(EDomain::Game, CFilePath("mods"));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (ReadFile with domain)", "[FS][FileSystem]")
{
	auto source = CreateMemorySource({
		{"texture.png", "pixel_data"},
		{"config.ini", "key=value"}
		});
	Mount(EDomain::Game, CFilePath("assets"), source);

	SECTION("Read existing file returns Success")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(EDomain::Game, CFilePath("assets/texture.png"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "pixel_data");
	}

	SECTION("Read another existing file returns Success")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(EDomain::Game, CFilePath("assets/config.ini"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "key=value");
	}

	SECTION("Read non-existing file returns NotFound")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(EDomain::Game, CFilePath("assets/missing.txt"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Read from wrong domain returns NotFound")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(EDomain::Engine, CFilePath("assets/texture.png"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	Unmount(EDomain::Game, CFilePath("assets"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (WriteFile with domain)", "[FS][FileSystem]")
{
	auto source = CreateMemorySource({ {"writable.txt", "old"} });
	Mount(EDomain::User, CFilePath("saves"), source);

	SECTION("Write to existing file and read back")
	{
		const std::string new_content = "new save data";
		CFileBuffer write_buffer(new_content.data(), static_cast<file_size_t>(new_content.size()));
		EFileResult result = WriteFile(EDomain::User, CFilePath("saves/writable.txt"), write_buffer);

		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		ReadFile(EDomain::User, CFilePath("saves/writable.txt"), read_buffer);
		CHECK(read_buffer.ToStringView() == new_content);
	}

	SECTION("Write to non-existing file returns AccessDenied")
	{
		CFileBuffer buffer("data", 4);
		EFileResult result = WriteFile(EDomain::User, CFilePath("saves/ghost.txt"), buffer);

		CHECK(result == EFileResult::AccessDenied);
	}

	SECTION("Write to wrong domain returns AccessDenied")
	{
		CFileBuffer buffer("data", 4);
		EFileResult result = WriteFile(EDomain::Engine, CFilePath("saves/writable.txt"), buffer);

		CHECK(result == EFileResult::AccessDenied);
	}

	Unmount(EDomain::User, CFilePath("saves"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (FileExists with domain)", "[FS][FileSystem]")
{
	auto source = CreateMemorySource({ {"shader.vert", "vertex"} });
	Mount(EDomain::Engine, CFilePath("shaders"), source);

	SECTION("Existing file returns true")
	{
		CHECK(FileExists(EDomain::Engine, CFilePath("shaders/shader.vert")));
	}

	SECTION("Non-existing file returns false")
	{
		CHECK(!FileExists(EDomain::Engine, CFilePath("shaders/missing.frag")));
	}

	SECTION("Wrong domain returns false")
	{
		CHECK(!FileExists(EDomain::Game, CFilePath("shaders/shader.vert")));
	}

	Unmount(EDomain::Engine, CFilePath("shaders"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (domain-less ReadFile)", "[FS][FileSystem]")
{
	auto game_source = CreateMemorySource({ {"shared.txt", "game_ver"} });
	auto engine_source = CreateMemorySource({ {"engine_only.txt", "engine_data"} });

	Mount(EDomain::Game, CFilePath("data"), game_source);
	Mount(EDomain::Engine, CFilePath("data"), engine_source);

	SECTION("ReadFile resolves via ResolveAny (Game first)")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(CFilePath("data/shared.txt"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "game_ver");
	}

	SECTION("ReadFile falls back to Engine domain")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(CFilePath("data/engine_only.txt"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "engine_data");
	}

	SECTION("ReadFile returns NotFound if nowhere")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(CFilePath("data/nowhere.txt"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	Unmount(EDomain::Game, CFilePath("data"));
	Unmount(EDomain::Engine, CFilePath("data"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (domain-less FileExists)", "[FS][FileSystem]")
{
	auto source = CreateMemorySource({ {"level.dat", "level_data"} });
	Mount(EDomain::Game, CFilePath("levels"), source);

	SECTION("FileExists finds file across domains")
	{
		CHECK(FileExists(CFilePath("levels/level.dat")));
	}

	SECTION("FileExists returns false for missing file")
	{
		CHECK(!FileExists(CFilePath("levels/missing.dat")));
	}

	Unmount(EDomain::Game, CFilePath("levels"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (GetMountSystem)", "[FS][FileSystem]")
{
	SECTION("GetMountSystem returns valid reference")
	{
		CMountSystem& ms = GetMountSystem();
		CHECK(ms.MountCount(EDomain::Game) == 0);
	}

	SECTION("GetMountSystem reflects mounts made through free functions")
	{
		auto source = std::make_shared<CFileSourceMemory>();
		Mount(EDomain::Dev, CFilePath("logs"), source);

		CHECK(GetMountSystem().HasMount(EDomain::Dev, CFilePath("logs")));
		CHECK(GetMountSystem().MountCount(EDomain::Dev) == 1);

		Unmount(EDomain::Dev, CFilePath("logs"));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (tag priority through free functions)", "[FS][FileSystem]")
{
	auto base_source = CreateMemorySource({ {"hero.lua", "base"} });
	auto patch_source = CreateMemorySource({ {"hero.lua", "patched"} });

	Mount(EDomain::Game, CFilePath("scripts"), base_source, EMountTag::Base);
	Mount(EDomain::Game, CFilePath("scripts"), patch_source, EMountTag::Patch);

	SECTION("Higher tag wins")
	{
		CFileBuffer buffer(0);
		EFileResult result = ReadFile(EDomain::Game, CFilePath("scripts/hero.lua"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "patched");
	}

	Unmount(EDomain::Game, CFilePath("scripts"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (cross-domain isolation)", "[FS][FileSystem]")
{
	auto game_source = CreateMemorySource({ {"game.dat", "game_data"} });
	auto engine_source = CreateMemorySource({ {"engine.dat", "engine_data"} });

	Mount(EDomain::Game, CFilePath("data"), game_source);
	Mount(EDomain::Engine, CFilePath("data"), engine_source);

	SECTION("Game domain cannot see Engine files")
	{
		CHECK(!FileExists(EDomain::Game, CFilePath("data/engine.dat")));
	}

	SECTION("Engine domain cannot see Game files")
	{
		CHECK(!FileExists(EDomain::Engine, CFilePath("data/game.dat")));
	}

	SECTION("Each domain reads its own file")
	{
		CFileBuffer game_buffer(0);
		ReadFile(EDomain::Game, CFilePath("data/game.dat"), game_buffer);
		CHECK(game_buffer.ToStringView() == "game_data");

		CFileBuffer engine_buffer(0);
		ReadFile(EDomain::Engine, CFilePath("data/engine.dat"), engine_buffer);
		CHECK(engine_buffer.ToStringView() == "engine_data");
	}

	Unmount(EDomain::Game, CFilePath("data"));
	Unmount(EDomain::Engine, CFilePath("data"));
}