#include "engine/engine_pch.hpp"

#include "TestsRTTIMock.hpp"

#include "engine/rtti/RTTITypeName.hpp"
#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::rtti;
using namespace engine::rtti::detail;
using namespace tests::rtti;

TEST_CASE("RTTI Type name", "[RTTI]")
{
    SECTION("Built-in base types")
	{
		constexpr std::string_view int_typename = CRTTITypeName::GetTypename<int>();
		REQUIRE(int_typename == "int");

		constexpr std::string_view float_typename = CRTTITypeName::GetTypename<float>();
		REQUIRE(float_typename == "float");

		constexpr std::string_view double_typename = CRTTITypeName::GetTypename<double>();
		REQUIRE(double_typename == "double");

		constexpr std::string_view char_typename = CRTTITypeName::GetTypename<char>();
		REQUIRE(char_typename == "char");

		constexpr std::string_view bool_typename = CRTTITypeName::GetTypename<bool>();
		REQUIRE(bool_typename == "bool");

		constexpr std::string_view void_typename = CRTTITypeName::GetTypename<void>();
		REQUIRE(void_typename == "void");
    }
    
    
    SECTION("Class types ok?")
	{
		constexpr std::string_view simple_struct_typename = CRTTITypeName::GetTypename<SSimpleStruct>();
		REQUIRE(simple_struct_typename == "tests::rtti::SSimpleStruct");

		constexpr std::string_view simple_class_typename = CRTTITypeName::GetTypename<CSimpleClass>();
		REQUIRE(simple_class_typename == "tests::rtti::CSimpleClass");

		constexpr std::string_view interface_typename = CRTTITypeName::GetTypename<IInterface>();
		REQUIRE(interface_typename == "tests::rtti::IInterface");

		constexpr std::string_view implementation_typename = CRTTITypeName::GetTypename<CImplementation>();
		REQUIRE(implementation_typename == "tests::rtti::CImplementation");
	}
}