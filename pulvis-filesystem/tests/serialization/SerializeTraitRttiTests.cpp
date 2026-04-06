#include "SerializeTraitTestsMoc.hpp"

#include "serialization/JsonArchiveBackend.hpp"
#include "serialization/BinaryArchiveBackend.hpp"
#include "serialization/SerializeTraitRTTI.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::fs;
using namespace pulvis::fs::serialization;
using namespace pulvis::fs::tests;

//////////////////////////////////////////////////////////////////////////
namespace
{
	template<typename T, typename BackendType>
	T RoundtripRTTI(const char* _name, const T& _original)
	{
		auto write_backend = std::make_unique<BackendType>();
		CSerializationArchive write_archive(std::move(write_backend), EArchiveMode::Write);
		write_archive.Process(_name, _original);
		CFileBuffer buffer = write_archive.GetResult();

		T result;
		auto read_backend = std::make_unique<BackendType>();
		CSerializationArchive read_archive(std::move(read_backend), EArchiveMode::Read);
		read_archive.SetInput(buffer);
		read_archive.Process(_name, result);

		return result;
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - RTTI class registration", "[FS][Serialization][RTTI]")
{
	SECTION("CTestComponent is found in storage")
	{
		CTestComponent instance;
		const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(instance.GetTypeId());

		REQUIRE(cls != nullptr);
		CHECK(cls->GetFields().size() == 6);
	}

	SECTION("CMinimalComponent is found in storage")
	{
		CMinimalComponent instance;
		const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(instance.GetTypeId());

		REQUIRE(cls != nullptr);
		CHECK(cls->GetFields().size() == 1);
	}

	SECTION("Fields have correct names")
	{
		CTestComponent instance;
		const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(instance.GetTypeId());

		REQUIRE(cls != nullptr);
		CHECK(cls->FindConstFieldByName("m_Health") != nullptr);
		CHECK(cls->FindConstFieldByName("m_Speed") != nullptr);
		CHECK(cls->FindConstFieldByName("m_Active") != nullptr);
		CHECK(cls->FindConstFieldByName("m_DamageMultiplier") != nullptr);
		CHECK(cls->FindConstFieldByName("m_Level") != nullptr);
		CHECK(cls->FindConstFieldByName("m_Name") != nullptr);
	}

	SECTION("Fields have correct ERTTIFieldTypes")
	{
		CTestComponent instance;
		const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(instance.GetTypeId());

		REQUIRE(cls != nullptr);
		CHECK(cls->FindConstFieldByName("m_Health")->GetFieldType() == pulvis::rtti::ERTTIFieldType::Int);
		CHECK(cls->FindConstFieldByName("m_Speed")->GetFieldType() == pulvis::rtti::ERTTIFieldType::Float);
		CHECK(cls->FindConstFieldByName("m_Active")->GetFieldType() == pulvis::rtti::ERTTIFieldType::Bool);
		CHECK(cls->FindConstFieldByName("m_DamageMultiplier")->GetFieldType() == pulvis::rtti::ERTTIFieldType::Double);
		CHECK(cls->FindConstFieldByName("m_Level")->GetFieldType() == pulvis::rtti::ERTTIFieldType::Uint32);
		CHECK(cls->FindConstFieldByName("m_Name")->GetFieldType() == pulvis::rtti::ERTTIFieldType::String);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - all field types (JSON)", "[FS][Serialization][RTTI][Json]")
{
	SECTION("Roundtrip with populated fields")
	{
		CTestComponent original;
		original.m_Health = 100;
		original.m_Speed = 5.5f;
		original.m_Active = true;
		original.m_DamageMultiplier = 1.75;
		original.m_Level = 42;
		original.m_Name = "Warrior";

		auto result = RoundtripRTTI<CTestComponent, CJsonArchiveBackend>("comp", original);

		CHECK(result.m_Health == 100);
		CHECK(result.m_Speed == Catch::Approx(5.5f));
		CHECK(result.m_Active == true);
		CHECK(result.m_DamageMultiplier == Catch::Approx(1.75));
		CHECK(result.m_Level == 42);
		CHECK(result.m_Name == "Warrior");
	}

	SECTION("Roundtrip with default values")
	{
		auto result = RoundtripRTTI<CTestComponent, CJsonArchiveBackend>("comp", CTestComponent{});

		CHECK(result.m_Health == 0);
		CHECK(result.m_Speed == Catch::Approx(0.0f));
		CHECK(result.m_Active == false);
		CHECK(result.m_DamageMultiplier == Catch::Approx(0.0));
		CHECK(result.m_Level == 0);
		CHECK(result.m_Name.empty());
	}

	SECTION("Negative values")
	{
		CTestComponent original;
		original.m_Health = -50;
		original.m_Speed = -1.0f;
		original.m_DamageMultiplier = -0.5;

		auto result = RoundtripRTTI<CTestComponent, CJsonArchiveBackend>("comp", original);

		CHECK(result.m_Health == -50);
		CHECK(result.m_Speed == Catch::Approx(-1.0f));
		CHECK(result.m_DamageMultiplier == Catch::Approx(-0.5));
	}

	SECTION("String with special characters")
	{
		CTestComponent original;
		original.m_Name = "Hero \"The Great\"\nLevel 99";

		auto result = RoundtripRTTI<CTestComponent, CJsonArchiveBackend>("comp", original);

		CHECK(result.m_Name == original.m_Name);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - all field types (Binary)", "[FS][Serialization][RTTI][Binary]")
{
	SECTION("Roundtrip with populated fields")
	{
		CTestComponent original;
		original.m_Health = 250;
		original.m_Speed = 9.99f;
		original.m_Active = false;
		original.m_DamageMultiplier = 3.14;
		original.m_Level = 99;
		original.m_Name = "Mage";

		auto result = RoundtripRTTI<CTestComponent, CBinaryArchiveBackend>("comp", original);

		CHECK(result.m_Health == 250);
		CHECK(result.m_Speed == Catch::Approx(9.99f));
		CHECK(result.m_Active == false);
		CHECK(result.m_DamageMultiplier == Catch::Approx(3.14));
		CHECK(result.m_Level == 99);
		CHECK(result.m_Name == "Mage");
	}

	SECTION("Default values")
	{
		auto result = RoundtripRTTI<CTestComponent, CBinaryArchiveBackend>("comp", CTestComponent{});

		CHECK(result.m_Health == 0);
		CHECK(result.m_Speed == Catch::Approx(0.0f));
		CHECK(result.m_Active == false);
		CHECK(result.m_DamageMultiplier == Catch::Approx(0.0));
		CHECK(result.m_Level == 0);
		CHECK(result.m_Name.empty());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - minimal component", "[FS][Serialization][RTTI]")
{
	SECTION("JSON roundtrip")
	{
		CMinimalComponent original;
		original.m_Value = 42;

		auto result = RoundtripRTTI<CMinimalComponent, CJsonArchiveBackend>("c", original);

		CHECK(result.m_Value == 42);
	}

	SECTION("Binary roundtrip")
	{
		CMinimalComponent original;
		original.m_Value = -999;

		auto result = RoundtripRTTI<CMinimalComponent, CBinaryArchiveBackend>("c", original);

		CHECK(result.m_Value == -999);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - __type field in JSON output", "[FS][Serialization][RTTI][Json]")
{
	SECTION("JSON output contains __type")
	{
		CTestComponent original;
		original.m_Health = 1;

		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(write_backend), EArchiveMode::Write);
		archive.Process("comp", original);

		CFileBuffer buffer = archive.GetResult();
		std::string json_str = buffer.ToString();

		CHECK(json_str.find("__type") != std::string::npos);
	}

	SECTION("JSON output contains all field names")
	{
		CTestComponent original;

		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(write_backend), EArchiveMode::Write);
		archive.Process("comp", original);

		CFileBuffer buffer = archive.GetResult();
		std::string json_str = buffer.ToString();

		CHECK(json_str.find("m_Health") != std::string::npos);
		CHECK(json_str.find("m_Speed") != std::string::npos);
		CHECK(json_str.find("m_Active") != std::string::npos);
		CHECK(json_str.find("m_DamageMultiplier") != std::string::npos);
		CHECK(json_str.find("m_Level") != std::string::npos);
		CHECK(json_str.find("m_Name") != std::string::npos);
	}

	SECTION("JSON output is valid parseable JSON")
	{
		CTestComponent original;
		original.m_Health = 50;

		auto write_backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(write_backend), EArchiveMode::Write);
		archive.Process("comp", original);

		CFileBuffer buffer = archive.GetResult();
		std::string json_str = buffer.ToString();

		CHECK_NOTHROW(nlohmann::json::parse(json_str));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - cross-backend consistency", "[FS][Serialization][RTTI]")
{
	SECTION("JSON and Binary produce equivalent deserialized results")
	{
		CTestComponent original;
		original.m_Health = 42;
		original.m_Speed = 7.77f;
		original.m_Active = true;
		original.m_Name = "CrossTest";
		original.m_DamageMultiplier = 2.5;
		original.m_Level = 10;

		auto json_result = RoundtripRTTI<CTestComponent, CJsonArchiveBackend>("comp", original);
		auto binary_result = RoundtripRTTI<CTestComponent, CBinaryArchiveBackend>("comp", original);

		CHECK(json_result.m_Health == binary_result.m_Health);
		CHECK(json_result.m_Speed == Catch::Approx(binary_result.m_Speed));
		CHECK(json_result.m_Active == binary_result.m_Active);
		CHECK(json_result.m_Name == binary_result.m_Name);
		CHECK(json_result.m_DamageMultiplier == Catch::Approx(binary_result.m_DamageMultiplier));
		CHECK(json_result.m_Level == binary_result.m_Level);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SerializeTraitRTTI - buffer is non-empty after serialization", "[FS][Serialization][RTTI]")
{
	SECTION("JSON produces non-empty buffer")
	{
		CMinimalComponent original;
		original.m_Value = 1;

		auto backend = std::make_unique<CJsonArchiveBackend>();
		CSerializationArchive archive(std::move(backend), EArchiveMode::Write);
		archive.Process("c", original);

		CHECK(!archive.GetResult().Empty());
	}

	SECTION("Binary produces non-empty buffer")
	{
		CMinimalComponent original;
		original.m_Value = 1;

		auto backend = std::make_unique<CBinaryArchiveBackend>();
		CSerializationArchive archive(std::move(backend), EArchiveMode::Write);
		archive.Process("c", original);

		CHECK(!archive.GetResult().Empty());
	}
}
