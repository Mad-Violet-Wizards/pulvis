#include "FileBuffer.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (construction)", "[FS][FileBuffer]")
{
	SECTION("Constructor with size")
	{
		CFileBuffer buffer(10);
		CHECK(buffer.Size() == 10);
		CHECK(!buffer.Empty());
	}

	SECTION("Constructor with zero size")
	{
		CFileBuffer buffer(0);
		CHECK(buffer.Size() == 0);
		CHECK(buffer.Empty());
	}

	SECTION("Constructor with data & size")
	{
		const char* data = "Hello";
		CFileBuffer buffer(data, 5);
		CHECK(buffer.Size() == 5);
		CHECK(!buffer.Empty());
	}

	SECTION("Constructor with binary data")
	{
		const uint8_t binary_data[] = { 0xDE, 0xAD, 0xBE, 0xEF };

		CFileBuffer buffer(binary_data, sizeof(binary_data));
		CHECK(buffer.Size() == sizeof(binary_data));
		CHECK(!buffer.Empty());
		CHECK(std::memcmp(buffer.Data(), binary_data, sizeof(binary_data)) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (copy)", "[FS][FileBuffer]")
{
	SECTION("Copy constructor")
	{
		CFileBuffer original("Hello", 5);
		CFileBuffer copy = original;
		CHECK(copy.Size() == original.Size());
		CHECK(std::memcmp(copy.Data(), original.Data(), static_cast<size_t>(original.Size())) == 0);
	}

	SECTION("Copy assignment operator")
	{
		CFileBuffer original("Hello", 5);
		CFileBuffer copy(0);
		copy = original;
		CHECK(copy.Size() == original.Size());
		CHECK(std::memcmp(copy.Data(), original.Data(), static_cast<size_t>(original.Size())) == 0);
	}

	SECTION("Copy is independent from original")
	{
		const char* data = "Independent";
		CFileBuffer original(data, 11);
		CFileBuffer copy = original;

		original.Clear();

		CHECK(original.Empty());
		CHECK(copy.Size() == 11);
		CHECK(copy.ToStringView() == "Independent");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (move)", "[FS][FileBuffer]")
{
	SECTION("Move constructor")
	{
		CFileBuffer original("Hello", 5);
		CFileBuffer moved = std::move(original);
		CHECK(moved.Size() == 5);
		CHECK(moved.ToStringView() == "Hello");
		CHECK(original.Empty());
	}
	SECTION("Move assignment operator")
	{
		CFileBuffer original("Hello", 5);
		CFileBuffer moved(0);
		moved = std::move(original);
		CHECK(moved.Size() == 5);
		CHECK(moved.ToStringView() == "Hello");
		CHECK(original.Empty());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (resize)", "[FS][FileBuffer]")
{
	SECTION("Resize larger")
	{
		CFileBuffer buffer(4);
		buffer.Resize(16);
		CHECK(buffer.Size() == 16);
	}

	SECTION("Resize smaller")
	{
		CFileBuffer buffer(16);
		buffer.Resize(4);
		CHECK(buffer.Size() == 4);
	}

	SECTION("Resize to zero")
	{
		CFileBuffer buffer(10);
		buffer.Resize(0);
		CHECK(buffer.Size() == 0);
		CHECK(buffer.Empty());
	}

	SECTION("Resize preserves existing data")
	{
		const char* data = "ABCD";
		CFileBuffer buffer(data, 4);
		buffer.Resize(8);

		CHECK(buffer.Size() == 8);
		CHECK(std::memcmp(buffer.Data(), data, 4) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (clear)", "[FS][FileBuffer]")
{
	SECTION("Clear buffer")
	{
		CFileBuffer buffer("Hello", 5);
		buffer.Clear();
		CHECK(buffer.Empty());
		CHECK(buffer.Size() == 0);
	}

	SECTION("Clear on already empty")
	{
		CFileBuffer buffer(0);
		buffer.Clear();
		CHECK(buffer.Empty());
		CHECK(buffer.Size() == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (append)", "[FS][FileBuffer]")
{
	SECTION("Append to empty buffer")
	{
		CFileBuffer buffer(0);
		const char* data = "Appended";
		buffer.Append(data, 8);

		CHECK(buffer.Size() == 8);
		CHECK(buffer.ToStringView() == "Appended");
	}

	SECTION("Append to existing data")
	{
		const char* initial = "Hello";
		CFileBuffer buffer(initial, 5);

		const char* extra = "World";
		buffer.Append(extra, 5);

		CHECK(buffer.Size() == 10);
		CHECK(buffer.ToStringView() == "HelloWorld");
	}

	SECTION("Multiple appends")
	{
		CFileBuffer buffer(0);

		buffer.Append("A", 1);
		buffer.Append("BB", 2);
		buffer.Append("CCC", 3);

		CHECK(buffer.Size() == 6);
		CHECK(buffer.ToStringView() == "ABBCCC");
	}

	SECTION("Append binary data")
	{
		CFileBuffer buffer(0);
		const uint8_t bin1[] = { 0x00, 0xFF };
		const uint8_t bin2[] = { 0xAB, 0xCD };

		buffer.Append(bin1, 2);
		buffer.Append(bin2, 2);

		CHECK(buffer.Size() == 4);
		CHECK(buffer.Data()[0] == 0x00);
		CHECK(buffer.Data()[1] == 0xFF);
		CHECK(buffer.Data()[2] == 0xAB);
		CHECK(buffer.Data()[3] == 0xCD);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("FileBuffer (conversions)", "[FS][FileBuffer]")
{
	SECTION("ToStringView")
	{
		const char* data = "StringView";
		CFileBuffer buffer(data, 10);
		CHECK(buffer.ToStringView() == "StringView");
	}

	SECTION("ToString")
	{
		const char* data = "ToString";
		CFileBuffer buffer(data, 8);
		std::string result = buffer.ToString();
		CHECK(result == "ToString");
	}

	SECTION("ToStringView on empty buffer")
	{
		CFileBuffer buffer(0);
		CHECK(buffer.ToStringView() == "");
		CHECK(buffer.ToStringView().empty());
	}

	SECTION("ToString on empty buffer")
	{
		CFileBuffer buffer(0);
		CHECK(buffer.ToString() == "");
		CHECK(buffer.ToString().empty());
	}

	SECTION("Data pointer is non-null for non-empty buffer")
	{
		CFileBuffer buffer(1);
		CHECK(buffer.Data() != nullptr);
	}

	SECTION("Const and non-const Data access")
	{
		const char* data = "RW";
		CFileBuffer buffer(data, 2);

		const CFileBuffer& const_ref = buffer;
		CHECK(const_ref.Data()[0] == 'R');

		buffer.Data()[0] = 'X';
		CHECK(buffer.ToStringView() == "XW");
	}
}
