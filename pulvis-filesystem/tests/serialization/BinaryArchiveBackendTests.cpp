#include "serialization/BinaryArchiveBackend.hpp"
#include "FileBuffer.hpp"

#include <catch2/catch2.hpp>

#include <cstring>

using namespace pulvis::fs;
using namespace pulvis::fs::serialization;

//////////////////////////////////////////////////////////////////////////
namespace
{
	CFileBuffer WriteThenRead(std::function<void(CBinaryArchiveBackend&)> _write,
		std::function<void(CBinaryArchiveBackend&)> _read)
	{
		CBinaryArchiveBackend writer;
		_write(writer);
		CFileBuffer buffer = writer.GetResult();

		CBinaryArchiveBackend reader;
		reader.SetInput(buffer);
		_read(reader);

		return buffer;
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (int roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Positive int64")
	{
		int64_t written = 123456789LL;
		int64_t read = 0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Negative int64")
	{
		int64_t written = -987654321LL;
		int64_t read = 0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Zero")
	{
		int64_t written = 0;
		int64_t read = 42;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);

		CHECK(read == 0);
	}

	SECTION("Min and max int64")
	{
		int64_t min_val = std::numeric_limits<int64_t>::min();
		int64_t max_val = std::numeric_limits<int64_t>::max();
		int64_t read_min = 0;
		int64_t read_max = 0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) {
				w.WriteInt(nullptr, min_val);
				w.WriteInt(nullptr, max_val);
			},
			[&](CBinaryArchiveBackend& r) {
				r.ReadInt(nullptr, read_min);
				r.ReadInt(nullptr, read_max);
			}
		);

		CHECK(read_min == min_val);
		CHECK(read_max == max_val);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (uint roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Positive uint64")
	{
		uint64_t written = 0xDEADBEEFCAFEBABEULL;
		uint64_t read = 0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Zero")
	{
		uint64_t written = 0;
		uint64_t read = 99;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);

		CHECK(read == 0);
	}

	SECTION("Max uint64")
	{
		uint64_t written = std::numeric_limits<uint64_t>::max();
		uint64_t read = 0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);

		CHECK(read == written);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (float roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Positive double")
	{
		double written = 3.14159265358979;
		double read = 0.0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteFloat(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadFloat(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Negative double")
	{
		double written = -2.71828;
		double read = 0.0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteFloat(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadFloat(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Zero")
	{
		double written = 0.0;
		double read = 1.0;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteFloat(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadFloat(nullptr, read); }
		);

		CHECK(read == 0.0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (bool roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("True")
	{
		bool written = true;
		bool read = false;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteBool(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadBool(nullptr, read); }
		);

		CHECK(read == true);
	}

	SECTION("False")
	{
		bool written = false;
		bool read = true;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteBool(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadBool(nullptr, read); }
		);

		CHECK(read == false);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (string roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Non-empty string")
	{
		std::string written = "Hello, Binary!";
		std::string read;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteString(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadString(nullptr, read); }
		);

		CHECK(read == written);
	}

	SECTION("Empty string")
	{
		std::string written;
		std::string read = "not_empty";

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteString(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadString(nullptr, read); }
		);

		CHECK(read.empty());
	}

	SECTION("String with special characters")
	{
		std::string written = "line1\nline2\ttab\0null";
		std::string read;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteString(nullptr, written); },
			[&](CBinaryArchiveBackend& r) { r.ReadString(nullptr, read); }
		);

		CHECK(read == written);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (raw roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Binary blob")
	{
		const uint8_t written[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0xFF };
		uint8_t read[6] = {};

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) { w.WriteRaw(nullptr, written, sizeof(written)); },
			[&](CBinaryArchiveBackend& r) { r.ReadRaw(nullptr, read, sizeof(read)); }
		);

		CHECK(std::memcmp(read, written, sizeof(written)) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (array roundtrip)", "[FS][Serialization][Binary]")
{
	SECTION("Array of ints")
	{
		const std::vector<int64_t> values = { 10, 20, 30, 40, 50 };
		std::vector<int64_t> read_values;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) {
				uint32_t count = static_cast<uint32_t>(values.size());
				w.BeginArray(nullptr, count, true);
				for (const auto& v : values)
					w.WriteInt(nullptr, v);
				w.EndArray();
			},
			[&](CBinaryArchiveBackend& r) {
				uint32_t count = 0;
				r.BeginArray(nullptr, count, false);
				read_values.resize(count);
				for (uint32_t i = 0; i < count; ++i)
					r.ReadInt(nullptr, read_values[i]);
				r.EndArray();
			}
		);

		REQUIRE(read_values.size() == values.size());
		for (size_t i = 0; i < values.size(); ++i)
			CHECK(read_values[i] == values[i]);
	}

	SECTION("Empty array")
	{
		WriteThenRead(
			[&](CBinaryArchiveBackend& w) {
				uint32_t count = 0;
				w.BeginArray(nullptr, count, true);
				w.EndArray();
			},
			[&](CBinaryArchiveBackend& r) {
				uint32_t count = 99;
				r.BeginArray(nullptr, count, false);
				r.EndArray();
				CHECK(count == 0);
			}
		);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (object - no framing)", "[FS][Serialization][Binary]")
{
	SECTION("BeginObject/EndObject do not add bytes")
	{
		CBinaryArchiveBackend writer;
		writer.BeginObject("test");
		writer.WriteInt(nullptr, 42);
		writer.EndObject();

		CFileBuffer with_object = writer.GetResult();

		CBinaryArchiveBackend writer2;
		writer2.WriteInt(nullptr, 42);

		CFileBuffer without_object = writer2.GetResult();

		CHECK(with_object.Size() == without_object.Size());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (mixed types sequential)", "[FS][Serialization][Binary]")
{
	SECTION("Multiple types in sequence")
	{
		int64_t w_int = -42;
		uint64_t w_uint = 1000;
		double w_float = 1.5;
		bool w_bool = true;
		std::string w_str = "pulvis";

		int64_t r_int = 0;
		uint64_t r_uint = 0;
		double r_float = 0.0;
		bool r_bool = false;
		std::string r_str;

		WriteThenRead(
			[&](CBinaryArchiveBackend& w) {
				w.WriteInt(nullptr, w_int);
				w.WriteUInt(nullptr, w_uint);
				w.WriteFloat(nullptr, w_float);
				w.WriteBool(nullptr, w_bool);
				w.WriteString(nullptr, w_str);
			},
			[&](CBinaryArchiveBackend& r) {
				r.ReadInt(nullptr, r_int);
				r.ReadUInt(nullptr, r_uint);
				r.ReadFloat(nullptr, r_float);
				r.ReadBool(nullptr, r_bool);
				r.ReadString(nullptr, r_str);
			}
		);

		CHECK(r_int == w_int);
		CHECK(r_uint == w_uint);
		CHECK(r_float == w_float);
		CHECK(r_bool == w_bool);
		CHECK(r_str == w_str);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("BinaryArchiveBackend (GetResult & SetInput)", "[FS][Serialization][Binary]")
{
	SECTION("GetResult returns non-empty buffer after writes")
	{
		CBinaryArchiveBackend writer;
		writer.WriteInt(nullptr, 1);
		CFileBuffer result = writer.GetResult();

		CHECK(!result.Empty());
		CHECK(result.Size() == sizeof(int64_t));
	}

	SECTION("GetResult on fresh backend returns empty")
	{
		CBinaryArchiveBackend writer;
		CFileBuffer result = writer.GetResult();

		CHECK(result.Empty());
	}
}