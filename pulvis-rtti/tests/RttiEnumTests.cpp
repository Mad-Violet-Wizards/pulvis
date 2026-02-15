#include "RttiEnumTestsMoc.hpp"
#include "RTTIEnum.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::rtti;
using namespace pulvis::rtti::tests;

TEST_CASE("Enum", "[RTTI][Enum]")
{
	SECTION("Name is extracted properly?")
	{
		static std::string_view enum_explicit_rtti = CRTTIEnum<EnumExplicit>::GetName();
		CHECK("pulvis::rtti::tests::EnumExplicit" == enum_explicit_rtti);
		static std::string_view num_implicit_rtti = CRTTIEnum<EnumImplicit>::GetName();
		CHECK("pulvis::rtti::tests::EnumImplicit" == num_implicit_rtti);
		static std::string_view enum_with_long_names_rtti = CRTTIEnum<EnumWithLongNames>::GetName();
		CHECK("pulvis::rtti::tests::EnumWithLongNames" == enum_with_long_names_rtti);
	}

	SECTION("Count is correct?")
	{
		static int enum_explicit_count = CRTTIEnum<EnumExplicit>::GetCount();
		CHECK(5 == enum_explicit_count);
		static int num_implicit_count = CRTTIEnum<EnumImplicit>::GetCount();
		CHECK(7 == num_implicit_count);
		static int enum_with_long_names_count = CRTTIEnum<EnumWithLongNames>::GetCount();
		CHECK(3 == enum_with_long_names_count);
	}

	SECTION("ToString works?")
	{
		CHECK("Value1" == CRTTIEnum<EnumExplicit>::ToString(EnumExplicit::Value1));
		CHECK("Value2" == CRTTIEnum<EnumExplicit>::ToString(EnumExplicit::Value2));
		CHECK("Value3" == CRTTIEnum<EnumExplicit>::ToString(EnumExplicit::Value3));
		CHECK("Value4" == CRTTIEnum<EnumExplicit>::ToString(EnumExplicit::Value4));
		CHECK("Value5" == CRTTIEnum<EnumExplicit>::ToString(EnumExplicit::Value5));

		CHECK("Value1" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value1));
		CHECK("Value2" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value2));
		CHECK("Value3" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value3));
		CHECK("Value4" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value4));
		CHECK("Value5" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value5));
		CHECK("Value6" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value6));
		CHECK("Value7" == CRTTIEnum<EnumImplicit>::ToString(EnumImplicit::Value7));

		CHECK("FirstValueWithALongName" == CRTTIEnum<EnumWithLongNames>::ToString(EnumWithLongNames::FirstValueWithALongName));
		CHECK("SecondValueIsAnValueWithAnEvenLongerName" == CRTTIEnum<EnumWithLongNames>::ToString(EnumWithLongNames::SecondValueIsAnValueWithAnEvenLongerName));
		CHECK("ThirdValueIsAnValueWithAnEvenLongerNameWhichMakesItPrettyLongYouWouldSayVeryLong" == CRTTIEnum<EnumWithLongNames>::ToString(EnumWithLongNames::ThirdValueIsAnValueWithAnEvenLongerNameWhichMakesItPrettyLongYouWouldSayVeryLong));
	}

	SECTION("From String works?")
	{
		static const EnumExplicit enum_explicit_first_value = CRTTIEnum<EnumExplicit>::FromString("Value1");
		CHECK(EnumExplicit::Value1 == enum_explicit_first_value);
		static const EnumExplicit enum_explicit_last_value = CRTTIEnum<EnumExplicit>::FromString("Value5");
		CHECK(EnumExplicit::Value5 == enum_explicit_last_value);

		static const EnumImplicit enum_implicit_first_value = CRTTIEnum<EnumImplicit>::FromString("Value1");
		CHECK(EnumImplicit::Value1 == enum_implicit_first_value);
		static const EnumImplicit enum_implicit_last_value = CRTTIEnum<EnumImplicit>::FromString("Value7");
		CHECK(EnumImplicit::Value7 == enum_implicit_last_value);

		static const EnumWithLongNames enum_with_long_names_first_value = CRTTIEnum<EnumWithLongNames>::FromString("FirstValueWithALongName");
		CHECK(EnumWithLongNames::FirstValueWithALongName == enum_with_long_names_first_value);
		static const EnumWithLongNames enum_with_long_names_last_value = CRTTIEnum<EnumWithLongNames>::FromString("ThirdValueIsAnValueWithAnEvenLongerNameWhichMakesItPrettyLongYouWouldSayVeryLong");
		CHECK(EnumWithLongNames::ThirdValueIsAnValueWithAnEvenLongerNameWhichMakesItPrettyLongYouWouldSayVeryLong == enum_with_long_names_last_value);
	}
}