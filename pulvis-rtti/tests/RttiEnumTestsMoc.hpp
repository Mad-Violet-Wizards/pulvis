#pragma once

namespace pulvis::rtti::tests
{
	enum class [[Reflection::Enum]] EnumExplicit
	{
		Value1 = 1,
		Value2 = 2,
		Value3 = 3,
		Value4 = 4,
		Value5 = 5
	};

	enum class [[Reflection::Enum]] EnumImplicit
	{
		Value1,
		Value2,
		Value3,
		Value4,
		Value5,
		Value6,
		Value7
	};

	enum class [[Reflection::Enum]] EnumWithLongNames
	{
		FirstValueWithALongName = 1,
		SecondValueIsAnValueWithAnEvenLongerName = 2,
		ThirdValueIsAnValueWithAnEvenLongerNameWhichMakesItPrettyLongYouWouldSayVeryLong = 3
	};
}