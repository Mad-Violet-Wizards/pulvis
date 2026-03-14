#include "FileSourceDisk.hpp"

#include <catch2/catch2.hpp>
#include <filesystem>
#include <fstream>

using namespace pulvis::fs;

namespace
{
	//////////////////////////////////////////////////////////////////////////
	class CTestDirectoryFixture
	{
		public:

			CTestDirectoryFixture()
				: m_TestDir(std::filesystem::temp_directory_path() / "pulvis_fs_disk_test")
			{
				std::filesystem::remove_all(m_TestDir);
				std::filesystem::create_directory(m_TestDir);
			}

			~CTestDirectoryFixture()
			{
				Clear();
			}

			const std::filesystem::path& Path() const { return m_TestDir; }

			void CreateFile(const std::string& _relativePath, const std::string& _content) const
			{
				std::filesystem::path file_path = m_TestDir / _relativePath;
				std::filesystem::create_directories(file_path.parent_path());
				std::ofstream ofs(file_path, std::ios::binary);
				ofs.write(_content.data(), static_cast<std::streamsize>(_content.size()));
			}

			void CreateSubdirectory(const std::string& _relativePath) const
			{
				std::filesystem::create_directories(m_TestDir / _relativePath);
			}

			void Clear()
			{
				std::filesystem::remove_all(m_TestDir);
			}

		private:

			std::filesystem::path m_TestDir;
	};
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (name & properties", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk disk_source(fixture.Path());

	SECTION("Name matches")
	{
		CHECK(disk_source.Name() == "Disk");
	}

	SECTION("IsReadOnly is false")
	{
		CHECK(!disk_source.IsReadOnly());
	}

	SECTION("Root matches constructor argument")
	{
		CHECK(disk_source.Root() == fixture.Path());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (exists)", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk disk_source(fixture.Path());

	SECTION("Existing file returns true")
	{
		fixture.CreateFile("test_exists.txt", "Hello");
		CFilePath path("test_exists.txt");
		CHECK(disk_source.Exists(path));
	}

	SECTION("Non-existing file returns false")
	{
		CFilePath path("non_existing_file.txt");
		CHECK(!disk_source.Exists(path));
	}

	SECTION("Existing directory returns true")
	{
		fixture.CreateSubdirectory("test_directory");
		CFilePath path("test_directory");
		CHECK(disk_source.Exists(path));
	}

	SECTION("Non-existing directory returns false")
	{
		CFilePath path("phantom_dir");
		CHECK(!disk_source.Exists(path));
	}

	SECTION("Nested file exists")
	{
		fixture.CreateFile("nested/dir/test_nested.txt", "Nested content");
		CFilePath path("nested/dir/test_nested.txt");
		CHECK(disk_source.Exists(path));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (read)", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk disk_source(fixture.Path());

	SECTION("Read existing file returns Success")
	{
		const std::string content = "File content for reading.";
		fixture.CreateFile("test_read.txt", content);
		CFilePath path("test_read.txt");

		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == content);
	}

	SECTION("Read non-existing file returns NotFound")
	{
		CFilePath path("non_existing_read.txt");
		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Read empty file returns Success with empty buffer")
	{
		fixture.CreateFile("empty_file.txt", "");
		CFilePath path("empty_file.txt");
		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.Size() == 0);
	}

	SECTION("Read binary data preserves content")
	{
		const std::string binary_content = { '\x00', '\xFF', '\xAB', '\xCD' };
		fixture.CreateFile("binary_file.bin", binary_content);

		CFilePath path("binary_file.bin");
		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(static_cast<size_t>(buffer.Size()) == binary_content.size());
		CHECK(std::memcmp(buffer.Data(), binary_content.data(), binary_content.size()) == 0);
	}

	SECTION("Read file in a subdirectory")
	{
		fixture.CreateFile("subdir/test_subdir.txt", "Subdirectory content");

		CFilePath path("subdir/test_subdir.txt");
		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == "Subdirectory content");
	}

	SECTION("Read large file")
	{
		const std::string large_content(1024 * 1024, 'A'); // 1 MB of 'A'
		fixture.CreateFile("large_file.txt", large_content);

		CFilePath path("large_file.txt");
		CFileBuffer buffer(0);
		EFileResult result = disk_source.Read(path, buffer);

		CHECK(result == EFileResult::Success);
		CHECK(buffer.ToStringView() == large_content);
		CHECK(static_cast<size_t>(buffer.Size()) == large_content.size());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (write)", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk source(fixture.Path());

	SECTION("Write to existing file overwrites content")
	{
		fixture.CreateFile("test_write.txt", "Original content");

		CFilePath path("test_write.txt");
		const std::string new_content = "New content after writing.";
		CFileBuffer buffer(new_content.data(), static_cast<file_size_t>(new_content.size()));
		EFileResult result = source.Write(path, buffer);
		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		source.Read(path, read_buffer);
		CHECK(read_buffer.ToStringView() == new_content);
	}

	SECTION("Write empty buffer to existing file")
	{
		fixture.CreateFile("test_write_empty.txt", "Content to be cleared");

		CFilePath path("test_write_empty.txt");
		CFileBuffer buffer(0); // Empty buffer
		EFileResult result = source.Write(path, buffer);

		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		source.Read(path, read_buffer);
		CHECK(read_buffer.Size() == 0);
	}

	SECTION("Write to non-existing file returns NotFound")
	{
		CFilePath path("does_not_exist.txt");
		const std::string content = "test";
		CFileBuffer buffer(content.data(), static_cast<file_size_t>(content.size()));

		EFileResult result = source.Write(path, buffer);
		CHECK(result == EFileResult::NotFound);
	}

	SECTION("Write binary data and read back")
	{
		const uint8_t binary[] = { 0x00, 0xFF, 0xAB, 0xCD, 0x00 };
		fixture.CreateFile("binary_rw.bin", "placeholder");

		CFilePath path("binary_rw.bin");
		CFileBuffer write_buffer(binary, 5);

		EFileResult result = source.Write(path, write_buffer);
		CHECK(result == EFileResult::Success);

		CFileBuffer read_buffer(0);
		source.Read(path, read_buffer);
		CHECK(read_buffer.Size() == 5);
		CHECK(std::memcmp(read_buffer.Data(), binary, 5) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (read-write roundtrip)", "[FS][FileSourceDisk]")
{
	CTestDirectoryFixture fixture;
	CFileSourceDisk source(fixture.Path());

	SECTION("Write then read returns identical content")
	{
		const std::string original = "roundtrip data";
		fixture.CreateFile("roundtrip.txt", original);

		CFilePath path("roundtrip.txt");

		CFileBuffer read_buffer(0);
		CHECK(source.Read(path, read_buffer) == EFileResult::Success);
		CHECK(read_buffer.ToStringView() == original);

		const std::string modified = "modified roundtrip content";
		CFileBuffer write_buffer(modified.data(), static_cast<file_size_t>(modified.size()));
		CHECK(source.Write(path, write_buffer) == EFileResult::Success);

		CFileBuffer verify_buffer(0);
		CHECK(source.Read(path, verify_buffer) == EFileResult::Success);
		CHECK(verify_buffer.ToStringView() == modified);
	}

	SECTION("Multiple writes to same file keep last content")
	{
		fixture.CreateFile("multi.txt", "initial");
		CFilePath path("multi.txt");

		const std::string write1 = "first write";
		CFileBuffer buf1(write1.data(), static_cast<file_size_t>(write1.size()));
		CHECK(source.Write(path, buf1) == EFileResult::Success);

		const std::string write2 = "second write";
		CFileBuffer buf2(write2.data(), static_cast<file_size_t>(write2.size()));
		CHECK(source.Write(path, buf2) == EFileResult::Success);

		CFileBuffer read_buffer(0);
		CHECK(source.Read(path, read_buffer) == EFileResult::Success);
		CHECK(read_buffer.ToStringView() == "second write");
	}
}

TEST_CASE("FileSourceDisk (GetFileInfo)", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk source(fixture.Path());

	SECTION("File info returns correct size and path")
	{
		const std::string content = "0123456789";
		fixture.CreateFile("info_test.txt", content);

		CFilePath path("info_test.txt");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);

		CHECK(result == EFileResult::Success);
		CHECK(info.Path == path);
		CHECK(info.Size == content.size());
		CHECK(!info.IsDirectory);
	}

	SECTION("Directory info returns correct properties")
	{
		fixture.CreateSubdirectory("info_dir");
		CFilePath path("info_dir");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);

		CHECK(result == EFileResult::Success);
		CHECK(info.Path == path);
		CHECK(info.IsDirectory);
		CHECK(info.Size == 0);
	}

	SECTION("Non-existing file returns NotFound")
	{
		CFilePath path("non_existing_info.txt");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("LastModified is a valid timestamp")
	{
		const std::string content = "timestamp test";
		fixture.CreateFile("timestamp.txt", content);
		CFilePath path("timestamp.txt");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);
		CHECK(result == EFileResult::Success);
		CHECK(info.LastModified > 0);
	}

	SECTION("Empty file has size of 0")
	{
		fixture.CreateFile("empty_info.txt", "");
		CFilePath path("empty_info.txt");
		SFileInfo info;
		EFileResult result = source.GetFileInfo(path, info);
		CHECK(result == EFileResult::Success);
		CHECK(info.Size == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileSourceDisk (ListDirectory)", "[FS][FileSourceDisk]")
{
	static CTestDirectoryFixture fixture;
	static CFileSourceDisk source(fixture.Path());

	SECTION("List directory with files")
	{
		fixture.CreateFile("a.txt", "aaa");
		fixture.CreateFile("b.txt", "bbb");

		CFilePath dir("");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::Success);
		CHECK(files.size() == 2);
	}

	SECTION("List directory with mixed files and subdirectories")
	{
		fixture.Clear();
		fixture.CreateFile("file.txt", "data");
		fixture.CreateSubdirectory("child");

		CFilePath dir("");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::Success);
		CHECK(files.size() == 2);

		bool has_file = false;
		bool has_dir = false;

		for (const auto& entry : files)
		{
			if (!entry.IsDirectory) has_file = true;
			if (entry.IsDirectory) has_dir = true;
		}

		CHECK(has_file);
		CHECK(has_dir);
	}

	SECTION("List directory entries have correct relative paths")
	{
		fixture.CreateFile("subdir/alpha.txt", "a");
		fixture.CreateFile("subdir/beta.txt", "b");

		CFilePath dir("subdir");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::Success);
		CHECK(files.size() == 2);

		for (const auto& entry : files)
		{
			CHECK(entry.Path.StartsWith("subdir/"));
		}
	}

	SECTION("List directory entries have correct sizes")
	{
		fixture.CreateFile("sized/small.txt", "abc");
		fixture.CreateFile("sized/large.txt", "abcdefghij");

		CFilePath dir("sized");
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

	SECTION("List empty directory returns NotFound")
	{
		fixture.CreateSubdirectory("empty_dir");

		CFilePath dir("empty_dir");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("List non-existing directory returns NotFound")
	{
		CFilePath dir("phantom_dir");
		std::vector<SFileInfo> files;
		EFileResult result = source.ListDirectory(dir, files);

		CHECK(result == EFileResult::NotFound);
	}

	SECTION("List does not recurse into subdirectories")
	{
		fixture.CreateFile("parent/child/deep.txt", "deep");
		fixture.CreateFile("parent/top.txt", "top");

		CFilePath dir("parent");
		std::vector<SFileInfo> files;
		source.ListDirectory(dir, files);

		// directory_iterator is non-recursive - expect "child/" dir and "top.txt"
		CHECK(files.size() == 2);
	}
}