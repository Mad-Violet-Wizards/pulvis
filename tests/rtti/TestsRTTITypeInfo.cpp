#include "engine/engine_pch.hpp"

#include "TestsRTTIMock.hpp"

#include "engine/rtti/RTTITypeInfo.hpp"
#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::rtti;
using namespace engine::rtti::detail;
using namespace tests::rtti;

TEST_CASE("Type info API", "[RTTI]")
{
	SECTION("Built-in base types?")
	{
		// Field Types
		constexpr ERTTIFieldType int_field_type = CRTTITypeInfo<int>::GetFieldType();
		REQUIRE(int_field_type == ERTTIFieldType::Int);

		constexpr ERTTIFieldType float_field_type = CRTTITypeInfo<float>::GetFieldType();
		REQUIRE(float_field_type == ERTTIFieldType::Float);

		constexpr ERTTIFieldType double_field_type = CRTTITypeInfo<double>::GetFieldType();
		REQUIRE(double_field_type == ERTTIFieldType::Double);

		constexpr ERTTIFieldType char_field_type = CRTTITypeInfo<char>::GetFieldType();
		REQUIRE(char_field_type == ERTTIFieldType::Char);

		constexpr ERTTIFieldType bool_field_type = CRTTITypeInfo<bool>::GetFieldType();
		REQUIRE(bool_field_type == ERTTIFieldType::Bool);

		constexpr ERTTIFieldType string_field_type = CRTTITypeInfo<std::string>::GetFieldType();
		REQUIRE(string_field_type == ERTTIFieldType::String);

		constexpr ERTTIFieldType long_field_type = CRTTITypeInfo<long>::GetFieldType();
		REQUIRE(long_field_type == ERTTIFieldType::Long);

		constexpr ERTTIFieldType long_long_field_type = CRTTITypeInfo<long long>::GetFieldType();
		REQUIRE(long_long_field_type == ERTTIFieldType::LongLong);

		ERTTIFieldType uint_field_type = CRTTITypeInfo<unsigned int>::GetFieldType();
		REQUIRE(uint_field_type == ERTTIFieldType::Uint);

		constexpr ERTTIFieldType uint8_field_type = CRTTITypeInfo<uint8_t>::GetFieldType();
		REQUIRE(uint8_field_type == ERTTIFieldType::Uint8);

		constexpr ERTTIFieldType uint16_field_type = CRTTITypeInfo<uint16_t>::GetFieldType();
		REQUIRE(uint16_field_type == ERTTIFieldType::Uint16);

		constexpr ERTTIFieldType uint32_field_type = CRTTITypeInfo<uint32_t>::GetFieldType();
		REQUIRE((uint32_field_type == ERTTIFieldType::Uint32 || uint32_field_type == ERTTIFieldType::Uint));

		constexpr ERTTIFieldType uint64_field_type = CRTTITypeInfo<uint64_t>::GetFieldType();
		REQUIRE(uint64_field_type == ERTTIFieldType::Uint64);

		constexpr ERTTIFieldType int_ptr_field_type = CRTTITypeInfo<int*>::GetFieldType();
		constexpr ERTTIFieldAccess int_ptr_field_access = CRTTITypeInfo<int*>::GetFieldAccess();
		REQUIRE(int_ptr_field_type == ERTTIFieldType::Int);
		REQUIRE(int_ptr_field_access == ERTTIFieldAccess::Pointer);

		constexpr ERTTIFieldType int_ref_field_type = CRTTITypeInfo<int&>::GetFieldType();
		constexpr ERTTIFieldAccess int_ref_field_access = CRTTITypeInfo<int&>::GetFieldAccess();
		REQUIRE(int_ref_field_type == ERTTIFieldType::Int);
		REQUIRE(int_ref_field_access == ERTTIFieldAccess::Reference);
	}
}