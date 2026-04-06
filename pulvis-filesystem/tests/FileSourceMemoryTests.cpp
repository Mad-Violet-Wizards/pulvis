#include "FileSourceMemory.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (name & properties)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory memory_source;
	SECTION("Name matches")
	{
		CHECK(memory_source.Name() == "Memory");
	}
	SECTION("IsReadOnly is false")
	{
		CHECK(memory_source.IsReadOnly() == false);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (store & exists)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("Stored file exists")
	{
		CFilePath path("textures/albedo.png");
		source.Store(path, CFileBuffer("data", 4));

		CHECK(source.Exists(path));
	}

	SECTION("Non-stored file does not exist")
	{
		CFilePath path("textures/ghost.png");

		CHECK(!source.Exists(path));
	}

	SECTION("Multiple stored files exist independently")
	{
		CFilePath path1("a.txt");
		CFilePath path2("b.txt");
		CFilePath path3("c.txt");

		source.Store(path1, CFileBuffer("aaa", 3));
		source.Store(path2, CFileBuffer("bbb", 3));

		CHECK(source.Exists(path1));
		CHECK(source.Exists(path2));
		CHECK(!source.Exists(path3));
	}

	SECTION("Store overwrites previous content")
	{
		CFilePath path("config.ini");
		source.Store(path, CFileBuffer("old", 3));
		source.Store(path, CFileBuffer("new_value", 9));

		CFileBuffer buffer(0);
		source.Read(path, buffer);
		CHECK(buffer.ToStringView() == "new_value");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (read)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("Read stored file returns Success")
	{
		const std::string content = "Hello, Memory!";
		CFilePath path("greetings.txt");
		source.Store(path, CFileBuffer(content.data(), static_cast<file_size_t>(content.size())));

		CFileBuffer buffer(0);
		EFileResult result = source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == content);
	}

	SECTION("Read non-existing file returns NotFound")
	{
		CFilePath path("missing.txt");
		CFileBuffer buffer(0);
		EFileResult result = source.Read(path, buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Read empty stored file returns Success with empty buffer")
	{
		CFilePath path("empty.txt");
		source.Store(path, CFileBuffer(0));

		CFileBuffer buffer(0);
		EFileResult result = source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.Size() == 0);
	}

	SECTION("Read binary data preserves content")
	{
		const uint8_t binary[] = { 0xDE, 0xAD, 0xBE, 0xEF };
		CFilePath path("binary.bin");
		source.Store(path, CFileBuffer(binary, sizeof(binary)));

		CFileBuffer buffer(0);
		EFileResult result = source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.Size() == sizeof(binary));
		CHECK(std::memcmp(buffer.Data(), binary, sizeof(binary)) == 0);
	}

	SECTION("Read returns a copy, not a reference")
	{
		CFilePath path("copy_test.txt");
		source.Store(path, CFileBuffer("original", 8));

		CFileBuffer buffer(0);
		source.Read(path, buffer);

		buffer.Data()[0] = 'X';

		CFileBuffer buffer2(0);
		source.Read(path, buffer2);
		CHECK(buffer2.ToStringView() == "original");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (write)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("Write to stored file overwrites content")
	{
		CFilePath path("writable.txt");
		source.Store(path, CFileBuffer("old", 3));

		const std::string new_content = "new content";
		CFileBuffer write_buffer(new_content.data(), static_cast<file_size_t>(new_content.size()));
		EFileResult result = source.Write(path, write_buffer);

		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		source.Read(path, read_buffer);
		CHECK(read_buffer.ToStringView() == new_content);
	}

	SECTION("Write to non-existing file returns NotFound")
	{
		CFilePath path("does_not_exist.txt");
		CFileBuffer buffer("test", 4);
		EFileResult result = source.Write(path, buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Write empty buffer clears content")
	{
		CFilePath path("to_clear.txt");
		source.Store(path, CFileBuffer("data", 4));

		CFileBuffer empty_buffer(0);
		EFileResult result = source.Write(path, empty_buffer);

		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		source.Read(path, read_buffer);
		CHECK(read_buffer.Size() == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (GetFileInfo)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("File info returns correct size and path")
	{
		const std::string content = "0123456789";
		CFilePath path("info.txt");
		source.Store(path, CFileBuffer(content.data(), static_cast<file_size_t>(content.size())));

		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);

		CHECK(result == EFileResult::Success);
		CHECK(info.Path == path);
		CHECK(info.Size == static_cast<file_size_t>(content.size()));
		CHECK(!info.IsDirectory);
	}

	SECTION("File info LastModified is 0 for memory source")
	{
		CFilePath path("timestamp.txt");
		source.Store(path, CFileBuffer("ts", 2));

		SFileInfo info;
		source.GetFileInfo(path, info);

		CHECK(info.LastModified == 0);
	}

	SECTION("Non-existing file returns NotFound")
	{
		CFilePath path("ghost.txt");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Empty file has size 0")
	{
		CFilePath path("empty.txt");
		source.Store(path, CFileBuffer(0));

		SFileInfo info;
		source.GetFileInfo(path, info);

		CHECK(info.Size == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (ListDirectory)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("List direct children only")
	{
		source.Store(CFilePath("assets/a.txt"), CFileBuffer("a", 1));
		source.Store(CFilePath("assets/b.txt"), CFileBuffer("b", 1));
		source.Store(CFilePath("assets/sub/deep.txt"), CFileBuffer("d", 1));

		CFilePath dir("assets");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::Success);
		CHECK(files.size() == 2);
	}

	SECTION("List root-level files with empty prefix")
	{
		source.Store(CFilePath("root1.txt"), CFileBuffer("r1", 2));
		source.Store(CFilePath("root2.txt"), CFileBuffer("r2", 2));

		CFilePath dir("");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::Success);
		CHECK(files.size() == 2);
	}

	SECTION("List empty directory returns NotFound")
	{
		source.Store(CFilePath("other/file.txt"), CFileBuffer("x", 1));

		CFilePath dir("empty_dir");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Listed entries have correct sizes")
	{
		source.Store(CFilePath("dir/small.txt"), CFileBuffer("abc", 3));
		source.Store(CFilePath("dir/large.txt"), CFileBuffer("abcdefghij", 10));

		CFilePath dir("dir");
		std::vector<SFileInfo> files;
		source.ListDirectory(dir, files);

		for (const auto& entry : files)
		{
			if (entry.Path.Filename() == "small.txt")
			{
				CHECK(entry.Size == 3);
			}
			else if (entry.Path.Filename() == "large.txt")
			{
				CHECK(entry.Size == 10);
			}
		}
	}

	SECTION("Listed entries are never directories")
	{
		source.Store(CFilePath("flat/one.txt"), CFileBuffer("1", 1));
		source.Store(CFilePath("flat/two.txt"), CFileBuffer("2", 1));
		source.Store(CFilePath("flat/deep/local"), CFileBuffer());

		CFilePath dir("flat");
		std::vector<SFileInfo> files;
		source.ListDirectory(dir, files);

		for (const auto& entry : files)
		{
			CHECK(!entry.IsDirectory);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceMemory (read-write roundtrip)", "[FS][FileSourceMemory]")
{
	CFileSourceMemory source;

	SECTION("Store, read, write, read back")
	{
		CFilePath path("roundtrip.txt");
		source.Store(path, CFileBuffer("initial", 7));

		CFileBuffer read_buffer(0);
		CHECK(source.Read(path, read_buffer) == EFileResult::Success);
		CHECK(read_buffer.ToStringView() == "initial");

		const std::string modified = "modified";
		CFileBuffer write_buffer(modified.data(), static_cast<file_size_t>(modified.size()));
		CHECK(source.Write(path, write_buffer) == EFileResult::Success);

		CFileBuffer verify_buffer(0);
		CHECK(source.Read(path, verify_buffer) == EFileResult::Success);
		CHECK(verify_buffer.ToStringView() == "modified");
	}
}