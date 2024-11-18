#pragma once

#include "engine/engine_pch.hpp"

#include "RTTITests.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

TEST_CASE("Type name tests", "[RTTI]")
{
	SECTION("Simple types ok?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

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
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		constexpr std::string_view simple_struct_typename = CRTTITypeName::GetTypename<SSimpleStruct>();
		REQUIRE(simple_struct_typename == "engine::rtti::tests::SSimpleStruct");

		constexpr std::string_view simple_class_typename = CRTTITypeName::GetTypename<CSimpleClass>();
		REQUIRE(simple_class_typename == "engine::rtti::tests::CSimpleClass");

		constexpr std::string_view interface_typename = CRTTITypeName::GetTypename<IInterface>();
		REQUIRE(interface_typename == "engine::rtti::tests::IInterface");

		constexpr std::string_view implementation_typename = CRTTITypeName::GetTypename<CImplementation>();
		REQUIRE(implementation_typename == "engine::rtti::tests::CImplementation");
	}
}

TEST_CASE("Hashing", "[RTTI]")
{
	SECTION("Hashing simple types?")
	{
		constexpr std::string_view int_typename = engine::rtti::CRTTITypeName::GetTypename<int>();
		constexpr engine::rtti::type_id_t int_hash = engine::rtti::Hash(int_typename.data());
		constexpr std::string_view int_typename2 = engine::rtti::CRTTITypeName::GetTypename<int>();
		constexpr engine::rtti::type_id_t int_hash2 = engine::rtti::Hash(int_typename2.data());
		REQUIRE(int_hash == int_hash2);
	}

	SECTION("Hashing class types?")
	{
		constexpr std::string_view simple_struct_typename = engine::rtti::CRTTITypeName::GetTypename<engine::rtti::tests::SSimpleStruct>();
		constexpr engine::rtti::type_id_t simple_struct_hash = engine::rtti::Hash(simple_struct_typename.data());
		constexpr std::string_view simple_struct_typename2 = engine::rtti::CRTTITypeName::GetTypename<engine::rtti::tests::SSimpleStruct>();
		constexpr engine::rtti::type_id_t simple_struct_hash2 = engine::rtti::Hash(simple_struct_typename2.data());
		REQUIRE(simple_struct_hash == simple_struct_hash2);

		constexpr std::string_view simple_class_typename = engine::rtti::CRTTITypeName::GetTypename<engine::rtti::tests::CSimpleClass>();
		constexpr engine::rtti::type_id_t simple_class_hash = engine::rtti::Hash(simple_class_typename.data());
		REQUIRE(simple_class_hash != simple_struct_hash);
	}

	SECTION("Hashing ref and ptr types?")
	{
		constexpr std::string_view int_ptr_typename = engine::rtti::CRTTITypeName::GetTypename<int*>();
		constexpr engine::rtti::type_id_t int_ptr_hash = engine::rtti::Hash(int_ptr_typename.data());
		constexpr std::string_view int_typename = engine::rtti::CRTTITypeName::GetTypename<int>();
		constexpr engine::rtti::type_id_t int_hash = engine::rtti::Hash(int_typename.data());
		REQUIRE(int_ptr_hash != int_hash);
	}
}

TEST_CASE("IsTypeOf", "[RTTI]")
{
	SECTION("Simple types ok?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		REQUIRE(CRTTITypeInfo<int>::IsTypeOfId(CRTTITypeInfo<int>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<float>::IsTypeOfId(CRTTITypeInfo<float>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<double>::IsTypeOfId(CRTTITypeInfo<double>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<char>::IsTypeOfId(CRTTITypeInfo<char>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<bool>::IsTypeOfId(CRTTITypeInfo<bool>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<void>::IsTypeOfId(CRTTITypeInfo<void>::GetTypeId()));
	}

	SECTION("Class types ok?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		CDerived derived;

		std::cout << derived.GetTypeId() << std::endl;
		std::cout << CRTTITypeInfo<CDerived>::GetTypeId() << std::endl; 
		std::cout << CRTTITypeInfo<CDerived2>::GetTypeId() << std::endl;
		std::cout << CRTTITypeInfo<IBase>::GetTypeId() << std::endl;

		REQUIRE(derived.GetTypeId() == CRTTITypeInfo<CDerived>::GetTypeId());
		REQUIRE(derived.IsTypeOfId(CRTTITypeInfo<CDerived>::GetTypeId()));
		REQUIRE(derived.IsTypeOfId(derived.GetTypeId()));
		REQUIRE(derived.IsTypeOf<CDerived>());
		REQUIRE(derived.IsTypeOf<IBase>());
		REQUIRE(derived.GetTypeId() != CRTTITypeInfo<CDerived2>::GetTypeId());
		REQUIRE(derived.IsTypeOf<CDerived2>() == false);
	}
}


TEST_CASE("Enum API", "[RTTI]")
{
	SECTION("Name is extracted properly?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		constexpr std::string_view rtti_enum_explicit_name = CRTTIEnum<EMocRttiEnumExplicit>::GetName();
	}

	SECTION("Count is ok?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		const int rtti_enum_explicit_count = CRTTIEnum<EMocRttiEnumExplicit>::GetCount();
		REQUIRE(rtti_enum_explicit_count == 6);

		const int rtti_enum_implicit_count = CRTTIEnum<EMocRttiEnumImplicit>::GetCount();
		REQUIRE(rtti_enum_implicit_count == 8);

		const int rtti_enum_long_names_count = CRTTIEnum<EMocRttiLongValuesNames>::GetCount();
		REQUIRE(rtti_enum_long_names_count == 6);

	}

	SECTION("To String?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		const std::string_view rtti_explicit_a_value = CRTTIEnum<EMocRttiEnumExplicit>::ToString(EMocRttiEnumExplicit::A);
		REQUIRE(rtti_explicit_a_value == "A");

		const std::string_view rtti_implicit_h_value = CRTTIEnum<EMocRttiEnumImplicit>::ToString(EMocRttiEnumImplicit::H);
		REQUIRE(rtti_implicit_h_value == "H");

		const std::string_view rtti_long_names_f_value = CRTTIEnum<EMocRttiLongValuesNames>::ToString(EMocRttiLongValuesNames::NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa);
		REQUIRE(rtti_long_names_f_value == "NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa");
	}

	SECTION("From String?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

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

TEST_CASE("RTTI Classes", "[RTTI]")
{
	SECTION("Classes are registered properly?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		CRTTIClass* rtti_class = CRTTIClass::FindInStorage("engine::rtti::tests::CDerived");
		REQUIRE(rtti_class != nullptr);
		REQUIRE(rtti_class->GetName() == "engine::rtti::tests::CDerived");

		CRTTIClass* rtti_class2 = CRTTIClass::FindInStorage("engine::rtti::tests::CDerived2");
		REQUIRE(rtti_class2 != nullptr);
		REQUIRE(rtti_class2->GetName() == "engine::rtti::tests::CDerived2");

		CRTTIClass* rtti_class3 = CRTTIClass::FindInStorage("engine::rtti::tests::IBase");
		REQUIRE(rtti_class3 != nullptr);
		REQUIRE(rtti_class3->GetName() == "engine::rtti::tests::IBase");
	}

	SECTION("Casting works?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::detail;
		using namespace engine::rtti::tests;

		IBase* base = new CDerived();
		IBase* base2 = new CDerived2();
		IBase* base3 = new CDerivedBoth();
		IBase2* base4 = new CDerivedBoth();

		base->Cast<CDerived>();
		base2->Cast<CDerived2>();
		base3->Cast<CDerivedBoth>();
		base4->Cast<CDerivedBoth>();

		REQUIRE(base != nullptr);
		REQUIRE(base2 != nullptr);
		REQUIRE(base3 != nullptr);
		REQUIRE(base4 != nullptr);

		REQUIRE(typeid(*base) == typeid(CDerived));
		REQUIRE(typeid(*base2) == typeid(CDerived2));
		REQUIRE(typeid(*base3) == typeid(CDerivedBoth));
		REQUIRE(typeid(*base4) == typeid(CDerivedBoth));

		delete base;
		delete base2;
		delete base3;
		delete base4;

	}
}

TEST_CASE("RTTI Methods", "[RTTI]")
{
	SECTION("Methods are registered and called properly?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::tests;

		CDerived derived;
		CDerived2 derived2;
		CDerivedBoth derived_both;

		CRTTIClass* rtti_class_base = CRTTIClass::FindInStorage("engine::rtti::tests::IBase");
		REQUIRE(rtti_class_base != nullptr);

		CRTTIMethod* rtti_method_add = rtti_class_base->FindMethodByName("Add");
		REQUIRE(rtti_method_add != nullptr);
		int add_result = rtti_method_add->Invoke<int>(&derived, 5, 3);
		REQUIRE(add_result == 8);

		CRTTIMethod* rtti_method_subtract = rtti_class_base->FindMethodByName("Subtract");
		REQUIRE(rtti_method_subtract != nullptr);
		int subtract_result = rtti_method_subtract->Invoke<int>(&derived, 5, 3);
		REQUIRE(subtract_result == 2);

		CRTTIClass* rtti_class_base2 = CRTTIClass::FindInStorage("engine::rtti::tests::IBase2");
		REQUIRE(rtti_class_base2 != nullptr);
		CRTTIMethod* rtti_method_multiply = rtti_class_base2->FindMethodByName("Multiply");
		REQUIRE(rtti_method_multiply != nullptr);
		int multiply_result = rtti_method_multiply->Invoke<int>(&derived_both, 5, 3);
		REQUIRE(multiply_result == 15);

		CRTTIMethod* rtti_method_divide = rtti_class_base2->FindMethodByName("Divide");
		REQUIRE(rtti_method_divide != nullptr);
		int divide_result = rtti_method_divide->Invoke<int>(&derived_both, 6, 3);
		REQUIRE(divide_result == 2);

		CRTTIClass* rtti_class_derived = CRTTIClass::FindInStorage("engine::rtti::tests::CDerived");
		CRTTIMethod* rtti_virtual_method = rtti_class_derived->FindMethodByName("GetVirtualBase");
		REQUIRE(rtti_virtual_method != nullptr);
		int virtual_result = rtti_virtual_method->Invoke<int>(&derived);
		REQUIRE(virtual_result == 1);
	}
}

TEST_CASE("RTTI Fields", "[RTTI]")
{
	SECTION("Fields are registered properly?")
	{
		using namespace engine::rtti;
		using namespace engine::rtti::tests;

		CDerived derived;
		CDerived2 derived2;
		CDerivedBoth derived_both;

		CRTTIClass* rtti_class_derived = CRTTIClass::FindInStorage("engine::rtti::tests::CDerived");
		REQUIRE(rtti_class_derived != nullptr);
		CRTTIField* rtti_field = rtti_class_derived->FindFieldByName("m_PrivateInt");
		REQUIRE(rtti_field != nullptr);
		rtti_field->Set(&derived, 10);
		REQUIRE(derived.GetPrivateInt() == 10);
		int result_get = 0;
		rtti_field->Get(&derived, result_get);
		REQUIRE(result_get == 10);
	}
}