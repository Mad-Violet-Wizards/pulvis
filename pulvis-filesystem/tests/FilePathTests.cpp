#include "FileSourceDisk.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Filepath (absolute paths)", "[FS][FilePath]")
{
	SECTION("Absolute paths work")
	{
		// No end backslash.
		CFilePath path("C:/Program Files/Windows");
		CHECK(path.Str() == "C:/Program Files/Windows");
		CHECK(path.CStr() == std::string_view("C:/Program Files/Windows"));

		// End backslash.
		CFilePath path2("C:/Program Files/Windows/");
		CHECK(path2.Str() == "C:/Program Files/Windows");
		CHECK(path2.CStr() == std::string_view("C:/Program Files/Windows"));
	}

	SECTION("Path extension (absolute)")
	{
		CFilePath path("C:/Program Files/Windows/notes.txt");
		CHECK(path.Extension() == ".txt");
		CFilePath path2("C:/Program Files/Windows/notes");
		CHECK(path2.Extension() == "");
		CFilePath path3("C:/Program Files/Windows/archive.tar.gz");
		CHECK(path3.Extension() == ".gz");
		CFilePath path4("C:/Program Files/Windows/.hiddenfile");
		CHECK(path4.Extension() == "");
		CFilePath path5("C:/Program Files/Windows/.hiddenfile.txt");
		CHECK(path5.Extension() == ".txt");
		CFilePath path6("C:/Program Files/Windows/complex.name.with.dots.txt");
		CHECK(path6.Extension() == ".txt");
		CFilePath path7("C:/Program Files/Windows/scripts/hero.lua");
		CHECK(path7.Extension() == ".lua");
	}

	SECTION("Path filename (absolute)")
	{
		CFilePath path("C:/Program Files/Windows/notes.txt");
		CHECK(path.Filename() == "notes.txt");
		CFilePath path2("C:/Program Files/Windows/notes");
		CHECK(path2.Filename() == "notes");
		CFilePath path3("C:/Program Files/Windows/archive.tar.gz");
		CHECK(path3.Filename() == "archive.tar.gz");
		CFilePath path4("C:/Program Files/Windows/.hiddenfile");
		CHECK(path4.Filename() == ".hiddenfile");
		CFilePath path5("C:/Program Files/Windows/.hiddenfile.txt");
		CHECK(path5.Filename() == ".hiddenfile.txt");
		CFilePath path6("C:/Program Files/Windows/complex.name.with.dots.txt");
		CHECK(path6.Filename() == "complex.name.with.dots.txt");
		CFilePath path7("C:/Program Files/Windows/scripts/hero.lua");
		CHECK(path7.Filename() == "hero.lua");
	}

	SECTION("Path directory (absolute)")
	{
		CFilePath path("C:/Program Files/Windows/notes.txt");
		CHECK(path.Directory() == "C:/Program Files/Windows");
		CFilePath path2("notes.txt");
		CHECK(path2.Directory() == "");
		CFilePath path3("C:/Program Files/Windows/archive.tar.gz");
		CHECK(path3.Directory() == "C:/Program Files/Windows");
		CFilePath path4("C:/Program Files/Windows/.hiddenfile");
		CHECK(path4.Directory() == "C:/Program Files/Windows");
		CFilePath path5("C:/Program Files/Windows/.hiddenfile.txt");
		CHECK(path5.Directory() == "C:/Program Files/Windows");
		CFilePath path6("C:/Program Files/Windows/complex.name.with.dots.txt");
		CHECK(path6.Directory() == "C:/Program Files/Windows");
		CFilePath path7("C:/Program Files/Windows/scripts/hero.lua");
		CHECK(path7.Directory() == "C:/Program Files/Windows/scripts");
	}

	SECTION("Starts with (absolute)")
	{
		CFilePath path("C:/Program Files/Windows/notes.txt");
		CHECK(path.StartsWith("C:/Program Files/"));
		CFilePath path2("C:/Program Files/Windows/notes.txt");
		CHECK(!path2.StartsWith("Program Files/"));
		CFilePath path3("C:/Program Files/Windows/notes.txt");
		CHECK(!path3.StartsWith("C:/Program Files/Windows/notes.txt/"));
	}

	SECTION("Operator / (absolute)")
	{
		CFilePath path("C:/Program Files/Windows");
		CFilePath combined = path / "scripts/hero.lua";
		CHECK(combined.Str() == "C:/Program Files/Windows/scripts/hero.lua");

		CFilePath path2("C:/Program Files/Windows");
		CFilePath combined2 = path2 / "/scripts/hero.lua";
		CHECK(combined2.Str() == "C:/Program Files/Windows/scripts/hero.lua");

		CFilePath path3("C:/Program Files/Windows/");
		CFilePath combined3 = path3 / "scripts/hero.lua";
		CHECK(combined3.Str() == "C:/Program Files/Windows/scripts/hero.lua");

		CFilePath path4("C:/Program Files/Windows/");
		CFilePath combined4 = path4 / "/scripts/hero.lua";
		CHECK(combined4.Str() == "C:/Program Files/Windows/scripts/hero.lua");
	}

	SECTION("Operator == (absolute)")
	{
		CFilePath path("E:/Projects/FantasticWorld/Assets/Shaders/alpha_blend.vert");
		CFilePath path2("E:/Projects/FantasticWorld/Assets/Shaders/alpha_blend.vert");
		CFilePath path3("E:/Projects/FantasticWorld/Assets/Scripts/hero.lua");
		CHECK(path == path2);
		CHECK(path != path3);
	}

	SECTION("Operator < (absolute)")
	{
		CFilePath path("E:/Projects/FantasticWorld/Assets/Shaders/alpha_blend.vert");
		CFilePath path2("E:/Projects/FantasticWorld/Assets/Shaders/beta_blend.vert");
		// Path2 is less than path, because 'b' > 'a' in ASCII.
		CHECK(path2 > path);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Filepath (relative paths)", "[FS][FilePath]")
{
	SECTION("Relative paths work")
	{
		CFilePath path("game/assets");
		CHECK(path.Str() == "game/assets");
		CHECK(path.CStr() == std::string_view("game/assets"));

		CFilePath path2("game/assets/");
		CHECK(path2.Str() == "game/assets");
		CHECK(path2.CStr() == std::string_view("game/assets"));
	}

	SECTION("Path extension (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CHECK(path.Extension() == ".png");

		CFilePath path2("game/assets/texture");
		CHECK(path2.Extension() == "");

		CFilePath path3("game/assets/archive.tar.gz");
		CHECK(path3.Extension() == ".gz");

		CFilePath path4("game/assets/.hiddenfile");
		CHECK(path4.Extension() == "");

		CFilePath path5("game/assets/.hiddenfile.txt");
		CHECK(path5.Extension() == ".txt");

		CFilePath path6("game/assets/complex.name.with.dots.txt");
		CHECK(path6.Extension() == ".txt");

		CFilePath path7("game/assets/scripts/hero.lua");
		CHECK(path7.Extension() == ".lua");
	}

	SECTION("Path filename (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CHECK(path.Filename() == "texture.png");

		CFilePath path2("game/assets/texture");
		CHECK(path2.Filename() == "texture");

		CFilePath path3("game/assets/archive.tar.gz");
		CHECK(path3.Filename() == "archive.tar.gz");

		CFilePath path4("game/assets/.hiddenfile");
		CHECK(path4.Filename() == ".hiddenfile");

		CFilePath path5("game/assets/.hiddenfile.txt");
		CHECK(path5.Filename() == ".hiddenfile.txt");

		CFilePath path6("game/assets/complex.name.with.dots.txt");
		CHECK(path6.Filename() == "complex.name.with.dots.txt");

		CFilePath path7("game/assets/scripts/hero.lua");
		CHECK(path7.Filename() == "hero.lua");
	}

	SECTION("Path directory (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CHECK(path.Directory() == "game/assets");

		CFilePath path2("texture.png");
		CHECK(path2.Directory() == "");

		CFilePath path3("game/assets/archive.tar.gz");
		CHECK(path3.Directory() == "game/assets");

		CFilePath path4("game/assets/.hiddenfile");
		CHECK(path4.Directory() == "game/assets");

		CFilePath path5("game/assets/.hiddenfile.txt");
		CHECK(path5.Directory() == "game/assets");

		CFilePath path6("game/assets/complex.name.with.dots.txt");
		CHECK(path6.Directory() == "game/assets");

		CFilePath path7("game/assets/scripts/hero.lua");
		CHECK(path7.Directory() == "game/assets/scripts");
	}

	SECTION("Starts with (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CHECK(path.StartsWith("game/"));

		CFilePath path2("game/assets/texture.png");
		CHECK(!path2.StartsWith("assets/"));

		CFilePath path3("game/assets/texture.png");
		CHECK(!path3.StartsWith("game/assets/texture.png/"));
	}

	SECTION("Operator / (relative)")
	{
		CFilePath path("game/assets");
		CFilePath combined = path / "texture.png";
		CHECK(combined.Str() == "game/assets/texture.png");

		CFilePath path2("game/assets/");
		CFilePath combined2 = path2 / "/scripts/hero.lua";
		CHECK(combined2.Str() == "game/assets/scripts/hero.lua");

		CFilePath path3("game/assets");
		CFilePath combined3 = path3 / "scripts/hero.lua";
		CHECK(combined3.Str() == "game/assets/scripts/hero.lua");
	}

	SECTION("Operator == (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CFilePath path2("game/assets/texture.png");
		CFilePath path3("game/assets/texture.jpg");
		CHECK(path == path2);
		CHECK(path != path3);
	}

	SECTION("Operator < (relative)")
	{
		CFilePath path("game/assets/texture.png");
		CFilePath path2("game/assets/texture.jpg");
		// Path2 is less than path, because 'p' > 'j' in ASCII.
		CHECK(path > path2);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Filepath (empty)", "[FS][FilePath]")
{
	SECTION("Empty path")
	{
		CFilePath path("");
		CFilePath path2;

		CHECK(path.Empty());
		CHECK(path2.Empty());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Filepath (hash)", "[FS][FilePath]")
{
	SECTION("Hashing works")
	{
		CFilePath path("game/assets/texture.png");
		CFilePath path2("game/assets/texture.png");
		CFilePath path3("game/assets/texture.jpg");
		std::hash<CFilePath> hasher;
		CHECK(hasher(path) == hasher(path2));
		CHECK(hasher(path) != hasher(path3));
	}

	SECTION("Hash in container")
	{
		std::unordered_map<CFilePath, std::string> path_map;

		CFilePath path1("game/assets/texture.png");
		CFilePath path2("game/assets/texture.jpg");

		path_map[path1] = "Texture PNG";
		path_map[path2] = "Texture JPG";
	}
}