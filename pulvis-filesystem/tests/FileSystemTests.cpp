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
TEST_CASE("FileSystem (mount & unmount)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	SECTION("Mount adds to MountSystem")
	{
		auto source = CreateMemorySource({ {"hero.lua", "print('hello')"} });
		fs.Mount(EDomain::Game, CFilePath("scripts"), source);

		CHECK(fs.GetMountSystem().HasMount(EDomain::Game, CFilePath("scripts")));
		CHECK(fs.GetMountSystem().MountCount(EDomain::Game) == 1);

		fs.Unmount(EDomain::Game, CFilePath("scripts"));
	}

	SECTION("Unmount removes from MountSystem")
	{
		auto source = CreateMemorySource({ {"data.txt", "content"} });
		fs.Mount(EDomain::Game, CFilePath("assets"), source);
		fs.Unmount(EDomain::Game, CFilePath("assets"));

		CHECK(!fs.GetMountSystem().HasMount(EDomain::Game, CFilePath("assets")));
		CHECK(fs.GetMountSystem().MountCount(EDomain::Game) == 0);
	}

	SECTION("Mount with custom tag")
	{
		auto source = CreateMemorySource({ {"mod.lua", "modded"} });
		fs.Mount(EDomain::Game, CFilePath("mods"), source, EMountTag::Mod);

		CHECK(fs.GetMountSystem().HasMount(EDomain::Game, CFilePath("mods")));

		fs.Unmount(EDomain::Game, CFilePath("mods"));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (ReadFile with domain)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto source = CreateMemorySource({
		{"texture.png", "pixel_data"},
		{"config.ini", "key=value"}
		});
	fs.Mount(EDomain::Game, CFilePath("assets"), source);

	SECTION("Read existing file returns Success")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(EDomain::Game, CFilePath("assets/texture.png"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "pixel_data");
	}

	SECTION("Read another existing file returns Success")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(EDomain::Game, CFilePath("assets/config.ini"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "key=value");
	}

	SECTION("Read non-existing file returns NotFound")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(EDomain::Game, CFilePath("assets/missing.txt"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Read from wrong domain returns NotFound")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(EDomain::Engine, CFilePath("assets/texture.png"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	fs.Unmount(EDomain::Game, CFilePath("assets"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (WriteFile with domain)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto source = CreateMemorySource({ {"writable.txt", "old"} });
	fs.Mount(EDomain::User, CFilePath("saves"), source);

	SECTION("Write to existing file and read back")
	{
		const std::string new_content = "new save data";
		CFileBuffer write_buffer(new_content.data(), static_cast<file_size_t>(new_content.size()));
		EFileResult result = fs.WriteFile(EDomain::User, CFilePath("saves/writable.txt"), write_buffer);

		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		fs.ReadFile(EDomain::User, CFilePath("saves/writable.txt"), read_buffer);
		CHECK(read_buffer.ToStringView() == new_content);
	}

	SECTION("Write to non-mounted file returns NotFound")
	{
		CFileBuffer buffer("data", 4);
		EFileResult result = fs.WriteFile(EDomain::User, CFilePath("saves/ghost.txt"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Write to wrong domain returns AccessDenied")
	{
		CFileBuffer buffer("data", 4);
		EFileResult result = fs.WriteFile(EDomain::Engine, CFilePath("saves/writable.txt"), buffer);

		CHECK(result == EFileResult::AccessDenied);
	}

	fs.Unmount(EDomain::User, CFilePath("saves"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (FileExists with domain)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto source = CreateMemorySource({ {"shader.vert", "vertex"} });
	fs.Mount(EDomain::Engine, CFilePath("shaders"), source);

	SECTION("Existing file returns true")
	{
		CHECK(fs.FileExists(EDomain::Engine, CFilePath("shaders/shader.vert")));
	}

	SECTION("Non-existing file returns false")
	{
		CHECK(!fs.FileExists(EDomain::Engine, CFilePath("shaders/missing.frag")));
	}

	SECTION("Wrong domain returns false")
	{
		CHECK(!fs.FileExists(EDomain::Game, CFilePath("shaders/shader.vert")));
	}

	fs.Unmount(EDomain::Engine, CFilePath("shaders"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (domain-less ReadFile)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto game_source = CreateMemorySource({ {"shared.txt", "game_ver"} });
	auto engine_source = CreateMemorySource({ {"engine_only.txt", "engine_data"} });

	fs.Mount(EDomain::Game, CFilePath("data"), game_source);
	fs.Mount(EDomain::Engine, CFilePath("data"), engine_source);

	SECTION("ReadFile resolves via ResolveAny (Game first)")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(CFilePath("data/shared.txt"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "game_ver");
	}

	SECTION("ReadFile falls back to Engine domain")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(CFilePath("data/engine_only.txt"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "engine_data");
	}

	SECTION("ReadFile returns NotFound if nowhere")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(CFilePath("data/nowhere.txt"), buffer);

		CHECK(result == EFileResult::NotFound);
	}

	fs.Unmount(EDomain::Game, CFilePath("data"));
	fs.Unmount(EDomain::Engine, CFilePath("data"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (domain-less FileExists)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto source = CreateMemorySource({ {"level.dat", "level_data"} });
	fs.Mount(EDomain::Game, CFilePath("levels"), source);

	SECTION("FileExists finds file across domains")
	{
		CHECK(fs.FileExists(CFilePath("levels/level.dat")));
	}

	SECTION("FileExists returns false for missing file")
	{
		CHECK(!fs.FileExists(CFilePath("levels/missing.dat")));
	}

	fs.Unmount(EDomain::Game, CFilePath("levels"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (GetMountSystem)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	SECTION("GetMountSystem returns valid reference")
	{
		CMountSystem& ms = fs.GetMountSystem();
		CHECK(ms.MountCount(EDomain::Game) == 0);
	}

	SECTION("GetMountSystem reflects mounts made through free functions")
	{
		auto source = std::make_shared<CFileSourceMemory>();
		fs.Mount(EDomain::Dev, CFilePath("logs"), source);

		CHECK(fs.GetMountSystem().HasMount(EDomain::Dev, CFilePath("logs")));
		CHECK(fs.GetMountSystem().MountCount(EDomain::Dev) == 1);

		fs.Unmount(EDomain::Dev, CFilePath("logs"));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (tag priority through free functions)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto base_source = CreateMemorySource({ {"hero.lua", "base"} });
	auto patch_source = CreateMemorySource({ {"hero.lua", "patched"} });

	fs.Mount(EDomain::Game, CFilePath("scripts"), base_source, EMountTag::Base);
	fs.Mount(EDomain::Game, CFilePath("scripts"), patch_source, EMountTag::Patch);

	SECTION("Higher tag wins")
	{
		CFileBuffer buffer(0);
		EFileResult result = fs.ReadFile(EDomain::Game, CFilePath("scripts/hero.lua"), buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "patched");
	}

	fs.Unmount(EDomain::Game, CFilePath("scripts"));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSystem (cross-domain isolation)", "[FS][FileSystem]")
{
	CFileSystem fs("pulvis-tests");

	auto game_source = CreateMemorySource({ {"game.dat", "game_data"} });
	auto engine_source = CreateMemorySource({ {"engine.dat", "engine_data"} });

	fs.Mount(EDomain::Game, CFilePath("data"), game_source);
	fs.Mount(EDomain::Engine, CFilePath("data"), engine_source);

	SECTION("Game domain cannot see Engine files")
	{
		CHECK(!fs.FileExists(EDomain::Game, CFilePath("data/engine.dat")));
	}

	SECTION("Engine domain cannot see Game files")
	{
		CHECK(!fs.FileExists(EDomain::Engine, CFilePath("data/game.dat")));
	}

	SECTION("Each domain reads its own file")
	{
		CFileBuffer game_buffer(0);
		fs.ReadFile(EDomain::Game, CFilePath("data/game.dat"), game_buffer);
		CHECK(game_buffer.ToStringView() == "game_data");

		CFileBuffer engine_buffer(0);
		fs.ReadFile(EDomain::Engine, CFilePath("data/engine.dat"), engine_buffer);
		CHECK(engine_buffer.ToStringView() == "engine_data");
	}

	fs.Unmount(EDomain::Game, CFilePath("data"));
	fs.Unmount(EDomain::Engine, CFilePath("data"));
}