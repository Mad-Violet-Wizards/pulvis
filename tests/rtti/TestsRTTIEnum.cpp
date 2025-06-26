#include "engine/engine_pch.hpp"

#include "TestsRTTIMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::rtti;
using namespace engine::rtti::detail;
using namespace tests::rtti;

TEST_CASE("Enum API", "[RTTI]")
{
	SECTION("Name is extracted properly?")
	{
		constexpr std::string_view rtti_enum_explicit_name = CRTTIEnum<EMocRttiEnumExplicit>::GetName();
	}

	SECTION("Count is ok?")
	{
		const int rtti_enum_explicit_count = CRTTIEnum<EMocRttiEnumExplicit>::GetCount();
		REQUIRE(rtti_enum_explicit_count == 6);

		const int rtti_enum_implicit_count = CRTTIEnum<EMocRttiEnumImplicit>::GetCount();
		REQUIRE(rtti_enum_implicit_count == 8);

		const int rtti_enum_long_names_count = CRTTIEnum<EMocRttiLongValuesNames>::GetCount();
		REQUIRE(rtti_enum_long_names_count == 6);

	}

	SECTION("To String?")
	{
		const std::string_view rtti_explicit_a_value = CRTTIEnum<EMocRttiEnumExplicit>::ToString(EMocRttiEnumExplicit::A);
		REQUIRE(rtti_explicit_a_value == "A");

		const std::string_view rtti_implicit_h_value = CRTTIEnum<EMocRttiEnumImplicit>::ToString(EMocRttiEnumImplicit::H);
		REQUIRE(rtti_implicit_h_value == "H");

		const std::string_view rtti_long_names_f_value = CRTTIEnum<EMocRttiLongValuesNames>::ToString(EMocRttiLongValuesNames::NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa);
		REQUIRE(rtti_long_names_f_value == "NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa");
	}

	SECTION("From String?")
	{
		constexpr std::string_view rtti_explicit_a = "A";
		const EMocRttiEnumExplicit rtti_explicit_a_value = CRTTIEnum<EMocRttiEnumExplicit>::FromString(rtti_explicit_a);
		REQUIRE(rtti_explicit_a_value == EMocRttiEnumExplicit::A);

		constexpr std::string_view rtti_implicit_h = "H";
		const EMocRttiEnumImplicit rtti_implicit_h_value = CRTTIEnum<EMocRttiEnumImplicit>::FromString(rtti_implicit_h);
		REQUIRE(rtti_implicit_h_value == EMocRttiEnumImplicit::H);

		constexpr std::string_view rtti_long_names_f = "NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa";
		const EMocRttiLongValuesNames rtti_long_names_f_value = CRTTIEnum<EMocRttiLongValuesNames>::FromString(rtti_long_names_f);
		REQUIRE(rtti_long_names_f_value == EMocRttiLongValuesNames::NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa);
	}
}