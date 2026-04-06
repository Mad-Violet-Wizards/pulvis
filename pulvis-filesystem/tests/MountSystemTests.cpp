#include "MountSystem.hpp"
#include "FileSourceMemory.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;

//////////////////////////////////////////////////////////////////////////
namespace
{
	std::shared_ptr<CFileSourceMemory> CreateMemorySource(std::initializer_list<std::pair<const char*, const char*>> files)
	{
		auto source = std::make_shared<CFileSourceMemory>();
		for (const auto& [path, content] : files)
		{
			source->Store(CFilePath(path), CFileBuffer(content, std::strlen(content)));
		}
		return source;
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (mount & unmount)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("Mount adds a mount point")
	{
		auto source = std::make_shared<CFileSourceMemory>();
		mount_system.Mount(EDomain::Game, CFilePath("assets/"), source);

		CHECK(mount_system.HasMount(EDomain::Game, CFilePath("assets/")));
		CHECK(mount_system.MountCount(EDomain::Game) == 1);
	}

	SECTION("Unmount removes a mount point")
	{
		auto source = std::make_shared<CFileSourceMemory>();
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);
		mount_system.Unmount(EDomain::Game, CFilePath("assets"));

		CHECK(!mount_system.HasMount(EDomain::Game, CFilePath("assets")));
		CHECK(mount_system.MountCount(EDomain::Game) == 0);
	}

	SECTION("Multiple mounts in same domain")
	{
		auto source1 = std::make_shared<CFileSourceMemory>();
		auto source2 = std::make_shared<CFileSourceMemory>();

		mount_system.Mount(EDomain::Game, CFilePath("assets"), source1);
		mount_system.Mount(EDomain::Game, CFilePath("mods"), source2);

		CHECK(mount_system.MountCount(EDomain::Game) == 2);
		CHECK(mount_system.HasMount(EDomain::Game, CFilePath("assets")));
		CHECK(mount_system.HasMount(EDomain::Game, CFilePath("mods")));
	}

	SECTION("Mount in different domains are independent")
	{
		auto source = std::make_shared<CFileSourceMemory>();

		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);
		mount_system.Mount(EDomain::Engine, CFilePath("shaders"), source);

		CHECK(mount_system.MountCount(EDomain::Game) == 1);
		CHECK(mount_system.MountCount(EDomain::Engine) == 1);
		CHECK(!mount_system.HasMount(EDomain::Game, CFilePath("shaders")));
		CHECK(!mount_system.HasMount(EDomain::Engine, CFilePath("assets")));
	}

	SECTION("Unmount only removes matching prefix")
	{
		auto source1 = std::make_shared<CFileSourceMemory>();
		auto source2 = std::make_shared<CFileSourceMemory>();

		mount_system.Mount(EDomain::Game, CFilePath("assets"), source1);
		mount_system.Mount(EDomain::Game, CFilePath("mods"), source2);
		mount_system.Unmount(EDomain::Game, CFilePath("assets"));

		CHECK(!mount_system.HasMount(EDomain::Game, CFilePath("assets")));
		CHECK(mount_system.HasMount(EDomain::Game, CFilePath("mods")));
		CHECK(mount_system.MountCount(EDomain::Game) == 1);
	}

	SECTION("Unmount non-existing path is safe")
	{
		mount_system.Unmount(EDomain::Game, CFilePath("non_existing"));
		CHECK(mount_system.MountCount(EDomain::Game) == 0);
	}

	SECTION("HasMount returns false for non-existing domain")
	{
		CHECK(!mount_system.HasMount(EDomain::Game, CFilePath("non_existing")));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (resolve)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("Resolve finds file in mounted source")
	{
		auto source = CreateMemorySource({ {"hero.lua", "print('hello')"} });
		mount_system.Mount(EDomain::Game, CFilePath("scripts"), source);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("scripts/hero.lua"), false);

		CHECK(resolved);
		CHECK(resolved.Source != nullptr);
		CHECK(resolved.LocalPath == CFilePath("hero.lua"));
	}

	SECTION("Resolve returns empty for non-existing file")
	{
		auto source = CreateMemorySource({ {"exists.txt", "data"} });
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("assets/missing.txt"), false);

		CHECK(!resolved);
		CHECK(resolved.Source == nullptr);
	}

	SECTION("Resolve returns empty for wrong domain")
	{
		auto source = CreateMemorySource({ {"config.ini", "key=val"} });
		mount_system.Mount(EDomain::Engine, CFilePath("config"), source);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("config/config.ini"), false);

		CHECK(!resolved);
	}

	SECTION("Resolve returns empty for wrong prefix")
	{
		auto source = CreateMemorySource({ {"texture.png", "pixels"} });
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("mods/texture.png"), false);

		CHECK(!resolved);
	}

	SECTION("Resolve with check_writable skips read-only sources")
	{
		auto source = CreateMemorySource({ {"data.txt", "content"} });
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);

		// CFileSourceMemory::IsReadOnly() returns false, so this should resolve.
		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("assets/data.txt"), true);

		CHECK(resolved);
	}

	SECTION("Resolve strips prefix correctly for nested paths")
	{
		auto source = CreateMemorySource({ {"sub/deep.txt", "deep"} });
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("assets/sub/deep.txt"), false);

		CHECK(resolved);
		CHECK(resolved.LocalPath == CFilePath("sub/deep.txt"));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (resolve priority / tags)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("Higher tag wins when multiple sources match")
	{
		auto base_source = CreateMemorySource({ {"hero.lua", "base"} });
		auto patch_source = CreateMemorySource({ {"hero.lua", "patched"} });

		mount_system.Mount(EDomain::Game, CFilePath("scripts"), base_source, EMountTag::Base);
		mount_system.Mount(EDomain::Game, CFilePath("scripts"), patch_source, EMountTag::Patch);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("scripts/hero.lua"), false);

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "patched");
	}

	SECTION("Mod tag overrides Patch tag")
	{
		auto patch_source = CreateMemorySource({ {"hero.lua", "patch"} });
		auto mod_source = CreateMemorySource({ {"hero.lua", "mod"} });

		mount_system.Mount(EDomain::Game, CFilePath("scripts"), patch_source, EMountTag::Patch);
		mount_system.Mount(EDomain::Game, CFilePath("scripts"), mod_source, EMountTag::Mod);

		auto resolved = mount_system.Resolve(EDomain::Game, CFilePath("scripts/hero.lua"), false);

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "mod");
	}

	SECTION("Base tag is used when no higher tag exists")
	{
		auto source = CreateMemorySource({ {"config.ini", "base_config"} });
		mount_system.Mount(EDomain::Engine, CFilePath("config"), source, EMountTag::Base);

		auto resolved = mount_system.Resolve(EDomain::Engine, CFilePath("config/config.ini"), false);

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "base_config");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (ResolveAll)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("ResolveAll returns all matching sources")
	{
		auto source1 = CreateMemorySource({ {"hero.lua", "v1"} });
		auto source2 = CreateMemorySource({ {"hero.lua", "v2"} });

		mount_system.Mount(EDomain::Game, CFilePath("scripts"), source1, EMountTag::Base);
		mount_system.Mount(EDomain::Game, CFilePath("scripts"), source2, EMountTag::Patch);

		auto resolved = mount_system.ResolveAll(EDomain::Game, CFilePath("scripts/hero.lua"));

		CHECK(resolved.size() == 2);
	}

	SECTION("ResolveAll returns empty when no match")
	{
		auto source = CreateMemorySource({ {"exists.txt", "data"} });
		mount_system.Mount(EDomain::Game, CFilePath("assets"), source);

		auto resolved = mount_system.ResolveAll(EDomain::Game, CFilePath("assets/missing.txt"));

		CHECK(resolved.empty());
	}

	SECTION("ResolveAll only returns sources that have the file")
	{
		auto source_with = CreateMemorySource({ {"shared.txt", "here"} });
		auto source_without = CreateMemorySource({ {"other.txt", "there"} });

		mount_system.Mount(EDomain::Game, CFilePath("data"), source_with, EMountTag::Base);
		mount_system.Mount(EDomain::Game, CFilePath("data"), source_without, EMountTag::Patch);

		auto resolved = mount_system.ResolveAll(EDomain::Game, CFilePath("data/shared.txt"));

		CHECK(resolved.size() == 1);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (ResolveAny)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("ResolveAny finds file in Game domain first")
	{
		auto game_source = CreateMemorySource({ {"shared.txt", "game"} });
		auto engine_source = CreateMemorySource({ {"shared.txt", "engine"} });

		mount_system.Mount(EDomain::Game, CFilePath("data"), game_source);
		mount_system.Mount(EDomain::Engine, CFilePath("data"), engine_source);

		auto resolved = mount_system.ResolveAny(CFilePath("data/shared.txt"));

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "game");
	}

	SECTION("ResolveAny falls back to Engine when Game has no match")
	{
		auto engine_source = CreateMemorySource({ {"shader.vert", "engine_shader"} });

		mount_system.Mount(EDomain::Engine, CFilePath("shaders"), engine_source);

		auto resolved = mount_system.ResolveAny(CFilePath("shaders/shader.vert"));

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "engine_shader");
	}

	SECTION("ResolveAny falls back to User domain")
	{
		auto user_source = CreateMemorySource({ {"settings.cfg", "user_settings"} });

		mount_system.Mount(EDomain::User, CFilePath("config"), user_source);

		auto resolved = mount_system.ResolveAny(CFilePath("config/settings.cfg"));

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "user_settings");
	}

	SECTION("ResolveAny falls back to Dev domain")
	{
		auto dev_source = CreateMemorySource({ {"debug.log", "dev_log"} });

		mount_system.Mount(EDomain::Dev, CFilePath("logs"), dev_source);

		auto resolved = mount_system.ResolveAny(CFilePath("logs/debug.log"));

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "dev_log");
	}

	SECTION("ResolveAny returns empty when nothing matches")
	{
		auto resolved = mount_system.ResolveAny(CFilePath("nowhere/file.txt"));

		CHECK(!resolved);
	}

	SECTION("ResolveAny respects domain order: Game > Engine > User > Dev")
	{
		auto game_source = CreateMemorySource({ {"priority.txt", "game"} });
		auto engine_source = CreateMemorySource({ {"priority.txt", "engine"} });
		auto user_source = CreateMemorySource({ {"priority.txt", "user"} });
		auto dev_source = CreateMemorySource({ {"priority.txt", "dev"} });

		mount_system.Mount(EDomain::Dev, CFilePath("test"), dev_source);
		mount_system.Mount(EDomain::User, CFilePath("test"), user_source);
		mount_system.Mount(EDomain::Engine, CFilePath("test"), engine_source);
		mount_system.Mount(EDomain::Game, CFilePath("test"), game_source);

		auto resolved = mount_system.ResolveAny(CFilePath("test/priority.txt"));

		CHECK(resolved);

		CFileBuffer buffer(0);
		resolved.Source->Read(resolved.LocalPath, buffer);
		CHECK(buffer.ToStringView() == "game");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MountSystem (MountCount & HasMount)", "[FS][MountSystem]")
{
	CMountSystem mount_system;

	SECTION("Empty system has zero mounts")
	{
		CHECK(mount_system.MountCount(EDomain::Game) == 0);
		CHECK(mount_system.MountCount(EDomain::Engine) == 0);
		CHECK(mount_system.MountCount(EDomain::User) == 0);
		CHECK(mount_system.MountCount(EDomain::Dev) == 0);
	}

	SECTION("MountCount increments on mount, decrements on unmount")
	{
		auto source = std::make_shared<CFileSourceMemory>();

		mount_system.Mount(EDomain::Game, CFilePath("a"), source);
		CHECK(mount_system.MountCount(EDomain::Game) == 1);

		mount_system.Mount(EDomain::Game, CFilePath("b"), source);
		CHECK(mount_system.MountCount(EDomain::Game) == 2);

		mount_system.Unmount(EDomain::Game, CFilePath("a"));
		CHECK(mount_system.MountCount(EDomain::Game) == 1);

		mount_system.Unmount(EDomain::Game, CFilePath("b"));
		CHECK(mount_system.MountCount(EDomain::Game) == 0);
	}
}