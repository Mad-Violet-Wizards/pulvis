#include "engine/engine_pch.hpp"

#include "TestsRTTIMock.hpp"
#include "engine/rtti/RTTIMethod.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::rtti;
using namespace engine::rtti::detail;
using namespace tests::rtti;

TEST_CASE("IsTypeOf", "[RTTI]")
{
	SECTION("Simple types ok?")
	{
		REQUIRE(CRTTITypeInfo<int>::IsTypeOfId(CRTTITypeInfo<int>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<float>::IsTypeOfId(CRTTITypeInfo<float>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<double>::IsTypeOfId(CRTTITypeInfo<double>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<char>::IsTypeOfId(CRTTITypeInfo<char>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<bool>::IsTypeOfId(CRTTITypeInfo<bool>::GetTypeId()));
		REQUIRE(CRTTITypeInfo<void>::IsTypeOfId(CRTTITypeInfo<void>::GetTypeId()));
	}

	SECTION("Class types ok?")
	{
		CDerived derived;

		REQUIRE(derived.GetTypeId() == CRTTITypeInfo<CDerived>::GetTypeId());
		REQUIRE(derived.IsTypeOfId(CRTTITypeInfo<CDerived>::GetTypeId()));
		REQUIRE(derived.IsTypeOfId(derived.GetTypeId()));
		REQUIRE(derived.IsTypeOf<CDerived>());
		REQUIRE(derived.IsTypeOf<IBase>());
		REQUIRE(derived.GetTypeId() != CRTTITypeInfo<CDerived2>::GetTypeId());
		REQUIRE(derived.IsTypeOf<CDerived2>() == false);
	}
}


TEST_CASE("RTTI Classes", "[RTTI]")
{
	SECTION("Classes are registered properly?")
	{
		CRTTIClass* rtti_class = CRTTIClass::FindInStorage("tests::rtti::CDerived");
		REQUIRE(rtti_class != nullptr);
		REQUIRE(rtti_class->GetName() == "tests::rtti::CDerived");

		CRTTIClass* rtti_class2 = CRTTIClass::FindInStorage("tests::rtti::CDerived2");
		REQUIRE(rtti_class2 != nullptr);
		REQUIRE(rtti_class2->GetName() == "tests::rtti::CDerived2");

		CRTTIClass* rtti_class3 = CRTTIClass::FindInStorage("tests::rtti::IBase");
		REQUIRE(rtti_class3 != nullptr);
		REQUIRE(rtti_class3->GetName() == "tests::rtti::IBase");
	}

	SECTION("Casting works?")
	{
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
		CDerived derived;
		CDerived2 derived2;
		CDerivedBoth derived_both;

		CRTTIClass* rtti_class_base = CRTTIClass::FindInStorage("tests::rtti::IBase");
		REQUIRE(rtti_class_base != nullptr);

		engine::rtti::CRTTIMethod* rtti_method_add = rtti_class_base->FindMethodByName("Add");
		REQUIRE(rtti_method_add != nullptr);
		int add_result = rtti_method_add->Invoke<int>(&derived, 5, 3);
		REQUIRE(add_result == 8);

		engine::rtti::CRTTIMethod* rtti_method_subtract = rtti_class_base->FindMethodByName("Subtract");
		REQUIRE(rtti_method_subtract != nullptr);
		int subtract_result = rtti_method_subtract->Invoke<int>(&derived, 5, 3);
		REQUIRE(subtract_result == 2);

		engine::rtti::CRTTIClass* rtti_class_base2 = CRTTIClass::FindInStorage("tests::rtti::IBase2");
		REQUIRE(rtti_class_base2 != nullptr);
		engine::rtti::CRTTIMethod* rtti_method_multiply = rtti_class_base2->FindMethodByName("Multiply");
		REQUIRE(rtti_method_multiply != nullptr);
		int multiply_result = rtti_method_multiply->Invoke<int>(&derived_both, 5, 3);
		REQUIRE(multiply_result == 15);

		engine::rtti::CRTTIMethod* rtti_method_divide = rtti_class_base2->FindMethodByName("Divide");
		REQUIRE(rtti_method_divide != nullptr);
		int divide_result = rtti_method_divide->Invoke<int>(&derived_both, 6, 3);
		REQUIRE(divide_result == 2);

		CRTTIClass* rtti_class_derived = CRTTIClass::FindInStorage("tests::rtti::CDerived");
		engine::rtti::CRTTIMethod* rtti_virtual_method = rtti_class_derived->FindMethodByName("GetVirtualBase");
		REQUIRE(rtti_virtual_method != nullptr);
		int virtual_result = rtti_virtual_method->Invoke<int>(&derived);
		REQUIRE(virtual_result == 1);
	}
}

TEST_CASE("RTTI Fields", "[RTTI]")
{
	SECTION("Fields are registered properly?")
	{
		CDerived derived;
		CDerived2 derived2;
		CDerivedBoth derived_both;

		CRTTIClass* rtti_class_derived = CRTTIClass::FindInStorage("tests::rtti::CDerived");
		REQUIRE(rtti_class_derived != nullptr);
		CRTTIField* rtti_field = rtti_class_derived->FindFieldByName("m_PrivateInt");
		REQUIRE(rtti_field != nullptr);
		rtti_field->Set(&derived, 10);
		REQUIRE(derived.GetPrivateInt() == 10);
		int result_get = 0;
		rtti_field->GetByRef(&derived, result_get);
		REQUIRE(result_get == 10);

		rtti_field->Set(&derived, 123);
		int result_get2 = rtti_field->Get<int>(&derived);
		REQUIRE(result_get2 == 123);
	}
}