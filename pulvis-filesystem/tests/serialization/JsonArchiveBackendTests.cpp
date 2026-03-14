#include "serialization/JsonArchiveBackend.hpp"
#include "FileBuffer.hpp"

#include <catch2/catch2.hpp>

#include <cstring>
#include <functional>

using namespace pulvis::fs;
using namespace pulvis::fs::serialization;

//////////////////////////////////////////////////////////////////////////
namespace
{
	void WriteThenRead(std::function<void(CJsonArchiveBackend&)> _write, std::function<void(CJsonArchiveBackend&)> _read)
	{
		CJsonArchiveBackend writer;
		_write(writer);
		CFileBuffer buffer = writer.GetResult();

		CJsonArchiveBackend reader;
		reader.SetInput(buffer);
		_read(reader);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (int roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Positive int64")
	{
		constexpr int64_t written = 123456789LL;
		int64_t read = 0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);
		CHECK(read == written);
	}
	SECTION("Negative int64")
	{
		constexpr int64_t written = -987654321LL;
		int64_t read = 0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);
		CHECK(read == written);
	}
	SECTION("Zero")
	{
		constexpr int64_t written = 0;
		int64_t read = 42;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadInt(nullptr, read); }
		);
		CHECK(read == 0);
	}

	SECTION("Max and min int64")
	{
		constexpr int64_t min_val = std::numeric_limits<int64_t>::min();
		constexpr int64_t max_val = std::numeric_limits<int64_t>::max();
		int64_t read_min = 0;
		int64_t read_max = 0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				w.WriteInt(nullptr, min_val);
				w.WriteInt(nullptr, max_val);
			},
			[&](CJsonArchiveBackend& r) {
				r.ReadInt(nullptr, read_min);
				r.ReadInt(nullptr, read_max);
			}
		);
		CHECK(read_min == min_val);
		CHECK(read_max == max_val);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (uint roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Positive uint64")
	{
		constexpr uint64_t written = 0xDEADBEEFCAFEBABEULL;
		uint64_t read = 0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);
		CHECK(read == written);
	}
	SECTION("Zero")
	{
		constexpr uint64_t written = 0;
		uint64_t read = 99;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);
		CHECK(read == 0);
	}
	SECTION("Max uint64")
	{
		constexpr uint64_t written = std::numeric_limits<uint64_t>::max();
		uint64_t read = 0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteUInt(nullptr, written); },
			[&](CJsonArchiveBackend& r) { r.ReadUInt(nullptr, read); }
		);
		CHECK(read == written);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (float roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Positive double")
	{
		double read = 0.0;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteFloat("pi", 3.14159265358979); },
			[&](CJsonArchiveBackend& r) { r.ReadFloat("pi", read); }
		);

		CHECK(read == 3.14159265358979);
	}

	SECTION("Negative double")
	{
		double read = 0.0;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteFloat("e", -2.71828); },
			[&](CJsonArchiveBackend& r) { r.ReadFloat("e", read); }
		);

		CHECK(read == -2.71828);
	}

	SECTION("MAX / MIN double")
	{
		double read_max = 0.0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteFloat("max", std::numeric_limits<double>::max()); },
			[&](CJsonArchiveBackend& r) { r.ReadFloat("max", read_max); }
		);
		CHECK(read_max == std::numeric_limits<double>::max());

		double read_min = 0.0;
		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteFloat("min", std::numeric_limits<double>::min()); },
			[&](CJsonArchiveBackend& r) { r.ReadFloat("min", read_min); }
		);
		CHECK(read_min == std::numeric_limits<double>::min());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (bool roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("True")
	{
		bool read = false;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteBool("flag", true); },
			[&](CJsonArchiveBackend& r) { r.ReadBool("flag", read); }
		);

		CHECK(read == true);
	}

	SECTION("False")
	{
		bool read = true;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteBool("flag", false); },
			[&](CJsonArchiveBackend& r) { r.ReadBool("flag", read); }
		);

		CHECK(read == false);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (string roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Non-empty string")
	{
		std::string read;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteString("msg", "Hello, JSON!"); },
			[&](CJsonArchiveBackend& r) { r.ReadString("msg", read); }
		);

		CHECK(read == "Hello, JSON!");
	}

	SECTION("Empty string")
	{
		std::string read = "not_empty";

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteString("msg", ""); },
			[&](CJsonArchiveBackend& r) { r.ReadString("msg", read); }
		);

		CHECK(read.empty());
	}

	SECTION("String with special characters")
	{
		const std::string written = "line1\nline2\ttab\"quote";
		std::string read;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteString("msg", written); },
			[&](CJsonArchiveBackend& r) { r.ReadString("msg", read); }
		);

		CHECK(read == written);
	}
}
//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (raw roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Binary blob as hex string")
	{
		constexpr uint8_t written[] = { 0xDE, 0xAD, 0xBE, 0xEF };
		uint8_t read[4] = {};

		WriteThenRead(
			[&](CJsonArchiveBackend& w) { w.WriteRaw("blob", written, sizeof(written)); },
			[&](CJsonArchiveBackend& r) { r.ReadRaw("blob", read, sizeof(read)); }
		);

		CHECK(std::memcmp(read, written, sizeof(written)) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (object roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Nested object with fields")
	{
		int64_t r_x = 0;
		int64_t r_y = 0;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				w.BeginObject("pos");
				w.WriteInt("x", 10);
				w.WriteInt("y", 20);
				w.EndObject();
			},
			[&](CJsonArchiveBackend& r) {
				r.BeginObject("pos");
				r.ReadInt("x", r_x);
				r.ReadInt("y", r_y);
				r.EndObject();
			}
		);

		CHECK(r_x == 10);
		CHECK(r_y == 20);
	}

	SECTION("Deeply nested objects")
	{
		std::string r_name;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				w.BeginObject("level1");
				w.BeginObject("level2");
				w.WriteString("name", "deep");
				w.EndObject();
				w.EndObject();
			},
			[&](CJsonArchiveBackend& r) {
				r.BeginObject("level1");
				r.BeginObject("level2");
				r.ReadString("name", r_name);
				r.EndObject();
				r.EndObject();
			}
		);

		CHECK(r_name == "deep");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (array roundtrip)", "[FS][Serialization][Json]")
{
	SECTION("Named array of ints")
	{
		const std::vector<int64_t> values = { 1, 2, 3, 4, 5 };
		std::vector<int64_t> read_values;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				uint32_t count = static_cast<uint32_t>(values.size());
				w.BeginArray("numbers", count, true);
				for (const auto& v : values)
					w.WriteInt(nullptr, v);
				w.EndArray();
			},
			[&](CJsonArchiveBackend& r) {
				uint32_t count = 0;
				r.BeginArray("numbers", count, false);
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
		uint32_t read_count = 99;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				uint32_t count = 0;
				w.BeginArray("empty", count, true);
				w.EndArray();
			},
			[&](CJsonArchiveBackend& r) {
				r.BeginArray("empty", read_count, false);
				r.EndArray();
			}
		);

		CHECK(read_count == 0);
	}

	SECTION("Array of strings")
	{
		const std::vector<std::string> values = { "alpha", "beta", "gamma" };
		std::vector<std::string> read_values;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				uint32_t count = static_cast<uint32_t>(values.size());
				w.BeginArray("names", count, true);
				for (const auto& v : values)
					w.WriteString(nullptr, v);
				w.EndArray();
			},
			[&](CJsonArchiveBackend& r) {
				uint32_t count = 0;
				r.BeginArray("names", count, false);
				read_values.resize(count);
				for (uint32_t i = 0; i < count; ++i)
					r.ReadString(nullptr, read_values[i]);
				r.EndArray();
			}
		);

		REQUIRE(read_values.size() == values.size());
		for (size_t i = 0; i < values.size(); ++i)
			CHECK(read_values[i] == values[i]);
	}

	SECTION("Array of objects")
	{
		struct Point { int64_t x; int64_t y; };
		const std::vector<Point> written = { {1, 2}, {3, 4}, {5, 6} };
		std::vector<Point> read_points;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				uint32_t count = static_cast<uint32_t>(written.size());
				w.BeginArray("points", count, true);
				for (const auto& p : written)
				{
					w.BeginObject(nullptr);
					w.WriteInt("x", p.x);
					w.WriteInt("y", p.y);
					w.EndObject();
				}
				w.EndArray();
			},
			[&](CJsonArchiveBackend& r) {
				uint32_t count = 0;
				r.BeginArray("points", count, false);
				read_points.resize(count);
				for (uint32_t i = 0; i < count; ++i)
				{
					r.BeginObject(nullptr);
					r.ReadInt("x", read_points[i].x);
					r.ReadInt("y", read_points[i].y);
					r.EndObject();
				}
				r.EndArray();
			}
		);

		REQUIRE(read_points.size() == written.size());
		for (size_t i = 0; i < written.size(); ++i)
		{
			CHECK(read_points[i].x == written[i].x);
			CHECK(read_points[i].y == written[i].y);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (multiple named fields)", "[FS][Serialization][Json]")
{
	SECTION("Mixed types at root level")
	{
		int64_t r_int = 0;
		double r_float = 0.0;
		bool r_bool = false;
		std::string r_str;

		WriteThenRead(
			[&](CJsonArchiveBackend& w) {
				w.WriteInt("health", 100);
				w.WriteFloat("speed", 5.5);
				w.WriteBool("alive", true);
				w.WriteString("name", "hero");
			},
			[&](CJsonArchiveBackend& r) {
				r.ReadInt("health", r_int);
				r.ReadFloat("speed", r_float);
				r.ReadBool("alive", r_bool);
				r.ReadString("name", r_str);
			}
		);

		CHECK(r_int == 100);
		CHECK(r_float == 5.5);
		CHECK(r_bool == true);
		CHECK(r_str == "hero");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JsonArchiveBackend (GetResult produces valid JSON)", "[FS][Serialization][Json]")
{
	SECTION("Output is parseable JSON")
	{
		CJsonArchiveBackend writer;
		writer.WriteInt("a", 1);
		writer.WriteString("b", "test");

		CFileBuffer result = writer.GetResult();
		std::string json_str = result.ToString();

		CHECK(!json_str.empty());
		CHECK_NOTHROW(nlohmann::json::parse(json_str));
	}

	SECTION("Output contains field names")
	{
		CJsonArchiveBackend writer;
		writer.WriteInt("my_field", 42);

		CFileBuffer result = writer.GetResult();
		std::string json_str = result.ToString();

		CHECK(json_str.find("my_field") != std::string::npos);
	}
}