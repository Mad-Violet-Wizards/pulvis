#include "SerializeTraitTestsMoc.hpp"

#include "serialization/JsonArchiveBackend.hpp"
#include "serialization/BinaryArchiveBackend.hpp"

#include "RTTIEnum.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;
using namespace pulvis::fs::serialization;
using namespace pulvis::fs::tests;

//////////////////////////////////////////////////////////////////////////
namespace
{
	template<typename T, typename Backend>
	T RoundtripValue(const char* _name, const T& _value)
	{
		auto write_backend = std::make_unique<Backend>();
		CSerializationArchive write_archive(std::move(write_backend), EArchiveMode::Write);
		write_archive.Process(_name, _value);
		CFileBuffer buffer = write_archive.GetResult();

		T result{};
		auto read_backend = std::make_unique<Backend>();
		CSerializationArchive read_archive(std::move(read_backend), EArchiveMode::Read);
		read_archive.SetInput(buffer);
		read_archive.Process(_name, result);

		return result;
	}
}

//////////////////////////////////////////////////////////////////////////
// Archive mode
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - archive mode", "[FS][Serialization][Trait]")
{
	SECTION("Write archive mode")
	{
		auto backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(backend), EArchiveMode::Write);

		CHECK(archive.IsWriteArchive());
		CHECK(!archive.IsReadArchive());
	}

	SECTION("Read archive mode")
	{
		auto backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(backend), EArchiveMode::Read);

		CHECK(archive.IsReadArchive());
		CHECK(!archive.IsWriteArchive());
	}
}

//////////////////////////////////////////////////////////////////////////
// Signed integers
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - signed integers (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("int32_t") { CHECK(RoundtripValue<int32_t, CJsonArchiveBackend>("v", -42) == -42); }
	SECTION("int64_t") { CHECK(RoundtripValue<int64_t, CJsonArchiveBackend>("v", -9876543210LL) == -9876543210LL); }
	SECTION("int16_t") { CHECK(RoundtripValue<int16_t, CJsonArchiveBackend>("v", int16_t(-1234)) == -1234); }
	SECTION("int8_t") { CHECK(RoundtripValue<int8_t, CJsonArchiveBackend>("v", int8_t(-42)) == -42); }
}

TEST_CASE("SerializeTrait - signed integers (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("int32_t") { CHECK(RoundtripValue<int32_t, CBinaryArchiveBackend>("v", -42) == -42); }
	SECTION("int64_t") { CHECK(RoundtripValue<int64_t, CBinaryArchiveBackend>("v", -9876543210LL) == -9876543210LL); }
}

//////////////////////////////////////////////////////////////////////////
// Unsigned integers
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - unsigned integers (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("uint32_t") { CHECK(RoundtripValue<uint32_t, CJsonArchiveBackend>("v", 42u) == 42u); }
	SECTION("uint64_t") { CHECK(RoundtripValue<uint64_t, CJsonArchiveBackend>("v", 0xDEADBEEFCAFEBABEULL) == 0xDEADBEEFCAFEBABEULL); }
	SECTION("uint16_t") { CHECK(RoundtripValue<uint16_t, CJsonArchiveBackend>("v", uint16_t(60000)) == 60000); }
	SECTION("uint8_t") { CHECK(RoundtripValue<uint8_t, CJsonArchiveBackend>("v", uint8_t(255)) == 255); }
}

TEST_CASE("SerializeTrait - unsigned integers (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("uint32_t") { CHECK(RoundtripValue<uint32_t, CBinaryArchiveBackend>("v", 42u) == 42u); }
	SECTION("uint64_t") { CHECK(RoundtripValue<uint64_t, CBinaryArchiveBackend>("v", 0xDEADBEEFCAFEBABEULL) == 0xDEADBEEFCAFEBABEULL); }
}

//////////////////////////////////////////////////////////////////////////
// Floating point
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - floating point (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("float") { CHECK(RoundtripValue<float, CJsonArchiveBackend>("v", 3.14f) == Catch::Approx(3.14f)); }
	SECTION("double") { CHECK(RoundtripValue<double, CJsonArchiveBackend>("v", 2.718281828459045) == 2.718281828459045); }
	SECTION("negative float") { CHECK(RoundtripValue<float, CJsonArchiveBackend>("v", -99.99f) == Catch::Approx(-99.99f)); }
	SECTION("zero") { CHECK(RoundtripValue<double, CJsonArchiveBackend>("v", 0.0) == 0.0); }
}

TEST_CASE("SerializeTrait - floating point (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("float") { CHECK(RoundtripValue<float, CBinaryArchiveBackend>("v", 3.14f) == Catch::Approx(3.14f)); }
	SECTION("double") { CHECK(RoundtripValue<double, CBinaryArchiveBackend>("v", 2.718281828459045) == 2.718281828459045); }
}

//////////////////////////////////////////////////////////////////////////
// Bool
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - bool (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("true") { CHECK(RoundtripValue<bool, CJsonArchiveBackend>("v", true) == true); }
	SECTION("false") { CHECK(RoundtripValue<bool, CJsonArchiveBackend>("v", false) == false); }
}

TEST_CASE("SerializeTrait - bool (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("true") { CHECK(RoundtripValue<bool, CBinaryArchiveBackend>("v", true) == true); }
	SECTION("false") { CHECK(RoundtripValue<bool, CBinaryArchiveBackend>("v", false) == false); }
}

//////////////////////////////////////////////////////////////////////////
// String
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - string (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("Non-empty") { CHECK(RoundtripValue<std::string, CJsonArchiveBackend>("v", std::string("Hello!")) == "Hello!"); }
	SECTION("Empty") { CHECK(RoundtripValue<std::string, CJsonArchiveBackend>("v", std::string()).empty()); }
}

TEST_CASE("SerializeTrait - string (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("Non-empty") { CHECK(RoundtripValue<std::string, CBinaryArchiveBackend>("v", std::string("Hello!")) == "Hello!"); }
	SECTION("Empty") { CHECK(RoundtripValue<std::string, CBinaryArchiveBackend>("v", std::string()).empty()); }
}


//////////////////////////////////////////////////////////////////////////
// Vector
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - vector (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("Vector of ints")
	{
		std::vector<int32_t> original = { 1, 2, 3, 4, 5 };
		auto result = RoundtripValue<std::vector<int32_t>, CJsonArchiveBackend>("arr", original);

		REQUIRE(result.size() == original.size());
		for (size_t i = 0; i < original.size(); ++i)
			CHECK(result[i] == original[i]);
	}

	SECTION("Empty vector")
	{
		auto result = RoundtripValue<std::vector<int32_t>, CJsonArchiveBackend>("arr", std::vector<int32_t>{});
		CHECK(result.empty());
	}

	SECTION("Vector of strings")
	{
		std::vector<std::string> original = { "alpha", "beta", "gamma" };
		auto result = RoundtripValue<std::vector<std::string>, CJsonArchiveBackend>("arr", original);

		REQUIRE(result.size() == original.size());
		for (size_t i = 0; i < original.size(); ++i)
			CHECK(result[i] == original[i]);
	}

	SECTION("Vector of doubles")
	{
		std::vector<double> original = { 1.1, 2.2, 3.3 };
		auto result = RoundtripValue<std::vector<double>, CJsonArchiveBackend>("arr", original);

		REQUIRE(result.size() == original.size());
		for (size_t i = 0; i < original.size(); ++i)
			CHECK(result[i] == original[i]);
	}
}

TEST_CASE("SerializeTrait - vector (Binary)", "[FS][Serialization][Trait][Binary]")
{
	SECTION("Vector of ints")
	{
		std::vector<int32_t> original = { 10, 20, 30 };
		auto result = RoundtripValue<std::vector<int32_t>, CBinaryArchiveBackend>("arr", original);

		REQUIRE(result.size() == original.size());
		for (size_t i = 0; i < original.size(); ++i)
			CHECK(result[i] == original[i]);
	}

	SECTION("Empty vector")
	{
		auto result = RoundtripValue<std::vector<int32_t>, CBinaryArchiveBackend>("arr", std::vector<int32_t>{});
		CHECK(result.empty());
	}
}

//////////////////////////////////////////////////////////////////////////
// Enum (via RTTI)
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - enum (JSON)", "[FS][Serialization][Trait][Json][Enum]")
{
	SECTION("Single enum value")
	{
		auto result = RoundtripValue<ETestElement, CJsonArchiveBackend>("elem", ETestElement::Water);
		CHECK(result == ETestElement::Water);
	}

	SECTION("All enum values")
	{
		const ETestElement all[] = { ETestElement::Fire, ETestElement::Water, ETestElement::Earth, ETestElement::Air };
		for (ETestElement e : all)
		{
			auto result = RoundtripValue<ETestElement, CJsonArchiveBackend>("elem", e);
			CHECK(result == e);
		}
	}

	SECTION("Enum stored as human-readable string")
	{
		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(write_backend), EArchiveMode::Write);
		ETestElement val = ETestElement::Earth;
		archive.Process("elem", val);

		CFileBuffer buffer = archive.GetResult();
		std::string json_str = buffer.ToString();

		CHECK(json_str.find("Earth") != std::string::npos);
	}
}

TEST_CASE("SerializeTrait - enum (Binary)", "[FS][Serialization][Trait][Binary][Enum]")
{
	SECTION("All enum values")
	{
		const ETestElement all[] = { ETestElement::Fire, ETestElement::Water, ETestElement::Earth, ETestElement::Air };
		for (ETestElement e : all)
		{
			auto result = RoundtripValue<ETestElement, CBinaryArchiveBackend>("elem", e);
			CHECK(result == e);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// PULVIS_SERIALIZE macro
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - PULVIS_SERIALIZE (JSON)", "[FS][Serialization][Trait][Json][Macro]")
{
	SECTION("Simple struct roundtrip")
	{
		SPlayerData original;
		original.Health = 100;
		original.Speed = 5.5f;
		original.Name = "Warrior";

		auto result = RoundtripValue<SPlayerData, CJsonArchiveBackend>("player", original);

		CHECK(result.Health == 100);
		CHECK(result.Speed == Catch::Approx(5.5f));
		CHECK(result.Name == "Warrior");
	}

	SECTION("Default values")
	{
		auto result = RoundtripValue<SPlayerData, CJsonArchiveBackend>("player", SPlayerData{});

		CHECK(result.Health == 0);
		CHECK(result.Speed == Catch::Approx(0.0f));
		CHECK(result.Name.empty());
	}

	SECTION("Struct with nested vector")
	{
		SInventoryItem original;
		original.Id = 42;
		original.Label = "Sword";
		original.Tags = { 1, 5, 10 };

		auto result = RoundtripValue<SInventoryItem, CJsonArchiveBackend>("item", original);

		CHECK(result.Id == 42);
		CHECK(result.Label == "Sword");
		REQUIRE(result.Tags.size() == 3);
		CHECK(result.Tags[0] == 1);
		CHECK(result.Tags[1] == 5);
		CHECK(result.Tags[2] == 10);
	}

	SECTION("Struct with empty vector")
	{
		SInventoryItem original;
		original.Id = 7;
		original.Label = "Empty";

		auto result = RoundtripValue<SInventoryItem, CJsonArchiveBackend>("item", original);

		CHECK(result.Id == 7);
		CHECK(result.Tags.empty());
	}
}

TEST_CASE("SerializeTrait - PULVIS_SERIALIZE (Binary)", "[FS][Serialization][Trait][Binary][Macro]")
{
	SECTION("Simple struct roundtrip")
	{
		SPlayerData original;
		original.Health = 250;
		original.Speed = 9.99f;
		original.Name = "Mage";

		auto result = RoundtripValue<SPlayerData, CBinaryArchiveBackend>("player", original);

		CHECK(result.Health == 250);
		CHECK(result.Speed == Catch::Approx(9.99f));
		CHECK(result.Name == "Mage");
	}

	SECTION("Struct with nested vector")
	{
		SInventoryItem original;
		original.Id = 99;
		original.Label = "Shield";
		original.Tags = { 10, 20, 30, 40 };

		auto result = RoundtripValue<SInventoryItem, CBinaryArchiveBackend>("item", original);

		CHECK(result.Id == 99);
		CHECK(result.Label == "Shield");
		REQUIRE(result.Tags.size() == 4);
		for (size_t i = 0; i < original.Tags.size(); ++i)
			CHECK(result.Tags[i] == original.Tags[i]);
	}
}

//////////////////////////////////////////////////////////////////////////
// Process const overload
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - const Process overload (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("Process with const reference serializes correctly")
	{
		const int32_t value = 777;

		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive write_archive(std::move(write_backend), EArchiveMode::Write);
		write_archive.Process("val", value);
		CFileBuffer buffer = write_archive.GetResult();

		int32_t read_value = 0;
		auto read_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive read_archive(std::move(read_backend), EArchiveMode::Read);
		read_archive.SetInput(buffer);
		read_archive.Process("val", read_value);

		CHECK(read_value == 777);
	}
}

//////////////////////////////////////////////////////////////////////////
// Multiple fields in single archive
//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTrait - multiple fields (JSON)", "[FS][Serialization][Trait][Json]")
{
	SECTION("Write and read multiple independent fields")
	{
		int32_t w_health = 100;
		float w_speed = 3.14f;
		std::string w_name = "pulvis";
		bool w_alive = true;

		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive write_archive(std::move(write_backend), EArchiveMode::Write);
		write_archive.Process("health", w_health);
		write_archive.Process("speed", w_speed);
		write_archive.Process("name", w_name);
		write_archive.Process("alive", w_alive);
		CFileBuffer buffer = write_archive.GetResult();

		int32_t r_health = 0;
		float r_speed = 0.0f;
		std::string r_name;
		bool r_alive = false;

		auto read_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive read_archive(std::move(read_backend), EArchiveMode::Read);
		read_archive.SetInput(buffer);
		read_archive.Process("health", r_health);
		read_archive.Process("speed", r_speed);
		read_archive.Process("name", r_name);
		read_archive.Process("alive", r_alive);

		CHECK(r_health == w_health);
		CHECK(r_speed == Catch::Approx(w_speed));
		CHECK(r_name == w_name);
		CHECK(r_alive == w_alive);
	}
}