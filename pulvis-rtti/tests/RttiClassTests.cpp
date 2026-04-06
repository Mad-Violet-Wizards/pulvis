#include "RttiClassTestsMoc.hpp"
#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "RTTITypeInfo.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::rtti;
using namespace pulvis::rtti::tests;

TEST_CASE("IsTypeOf", "[RTTI][Utility]")
{
	SECTION("Simple types ok?")
	{
		CHECK(CRTTITypeInfo<int>::IsTypeOfId(CRTTITypeInfo<int>::GetTypeId()));
		CHECK(CRTTITypeInfo<float>::IsTypeOfId(CRTTITypeInfo<float>::GetTypeId()));
		CHECK(CRTTITypeInfo<double>::IsTypeOfId(CRTTITypeInfo<double>::GetTypeId()));
		CHECK(CRTTITypeInfo<bool>::IsTypeOfId(CRTTITypeInfo<bool>::GetTypeId()));
		CHECK(CRTTITypeInfo<char>::IsTypeOfId(CRTTITypeInfo<char>::GetTypeId()));
		CHECK(CRTTITypeInfo<std::string>::IsTypeOfId(CRTTITypeInfo<std::string>::GetTypeId()));
		CHECK(CRTTITypeInfo<long>::IsTypeOfId(CRTTITypeInfo<long>::GetTypeId()));
		CHECK(CRTTITypeInfo<long long>::IsTypeOfId(CRTTITypeInfo<long long>::GetTypeId()));
		CHECK(CRTTITypeInfo<unsigned int>::IsTypeOfId(CRTTITypeInfo<unsigned int>::GetTypeId()));
		CHECK(CRTTITypeInfo<unsigned long>::IsTypeOfId(CRTTITypeInfo<unsigned long>::GetTypeId()));
		CHECK(CRTTITypeInfo<unsigned long long>::IsTypeOfId(CRTTITypeInfo<unsigned long long>::GetTypeId()));
		CHECK(CRTTITypeInfo<void>::IsTypeOfId(CRTTITypeInfo<void>::GetTypeId()));

		CHECK(!CRTTITypeInfo<int>::IsTypeOfId(CRTTITypeInfo<float>::GetTypeId()));
		CHECK(!CRTTITypeInfo<std::string>::IsTypeOfId(CRTTITypeInfo<char>::GetTypeId()));
	}

	SECTION("Class types ok?")
	{
		CDerived derived;
		CDerived2 derived2;
		CDerivedBoth derived_both;

		CHECK(derived.GetTypeId() == CRTTITypeInfo<CDerived>::GetTypeId());
		CHECK(derived2.GetTypeId() == CRTTITypeInfo<CDerived2>::GetTypeId());
		CHECK(derived_both.GetTypeId() == CRTTITypeInfo<CDerivedBoth>::GetTypeId());
		CHECK(derived.GetTypeId() != CRTTITypeInfo<CDerived2>::GetTypeId());
		CHECK(derived.GetTypeId() != CRTTITypeInfo<CDerivedBoth>::GetTypeId());
		CHECK(derived2.GetTypeId() != CRTTITypeInfo<CDerived>::GetTypeId());
		CHECK(derived2.GetTypeId() != CRTTITypeInfo<CDerivedBoth>::GetTypeId());
		CHECK(derived_both.GetTypeId() != CRTTITypeInfo<CDerived>::GetTypeId());
		CHECK(derived_both.GetTypeId() != CRTTITypeInfo<CDerived2>::GetTypeId());

		CHECK(derived.IsTypeOf<IBase>());
		CHECK(derived2.IsTypeOf<IBase2>());
	}
}

TEST_CASE("Class", "[RTTI][Class]")
{
	SECTION("Register in storage works?")
	{
		const char* derived_name = "pulvis::rtti::tests::CDerived";
		CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
		CHECK(derived_rtti_instance != nullptr);
		CHECK(derived_rtti_instance->GetName() == derived_name);

		const char* derived2_name = "pulvis::rtti::tests::CDerived2";
		CRTTIClass* derived2_rtti_instance = CRTTIClass::FindInStorage(derived2_name);
		CHECK(derived2_rtti_instance != nullptr);
		CHECK(derived2_rtti_instance->GetName() == derived2_name);

		const char* derived_both_name = "pulvis::rtti::tests::CDerivedBoth";
		CRTTIClass* derived_both_rtti_instance = CRTTIClass::FindInStorage(derived_both_name);
		CHECK(derived_both_rtti_instance != nullptr);
		CHECK(derived_both_rtti_instance->GetName() == derived_both_name);
	}

	SECTION("Casting works?")
	{
		IBase* derived = new CDerived();
		IBase2* derived2 = new CDerived2();
		IBase* derived_both = new CDerivedBoth();
		IBase2* derived_both_2 = new CDerivedBoth();

		CHECK(derived->GetVirtualBase() == 2);
		CHECK(derived2->GetVirtualBase() == 3);
		CHECK(derived_both->GetVirtualBase() == 4);
		CHECK(derived_both_2->GetVirtualBase() == 4);

		CDerived* d = derived->Cast<CDerived>();
		d->IntField = 1;

		CDerived2* d2 = derived2->Cast<CDerived2>();
		d2->IntField = 2;

		CDerivedBoth* db = derived_both->Cast<CDerivedBoth>();
		db->IntField = 3;

		CDerivedBoth* db2 = derived_both_2->Cast<CDerivedBoth>();
		db2->IntField = 4;

		CHECK(d->IntField == 1);
		CHECK(d2->IntField == 2);
		CHECK(db->IntField == 3);
		CHECK(db2->IntField == 4);
	}
}

TEST_CASE("Method", "[RTTI][Method]")
{
	SECTION("Finding method works?")
	{
		const char* derived_name = "pulvis::rtti::tests::CDerived";
		CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
		CHECK(derived_rtti_instance != nullptr);
		const char* method_name = "GetVirtualBase";
		CRTTIMethod* method = derived_rtti_instance->FindMethodByName(method_name);
		CHECK(method != nullptr);
		const char* add_method_name = "Add";
		CRTTIMethod* add_method = derived_rtti_instance->FindMethodByName(add_method_name);
		CHECK(add_method != nullptr);

		const char* derived2_name = "pulvis::rtti::tests::CDerived2";
		CRTTIClass* derived2_rtti_instance = CRTTIClass::FindInStorage(derived2_name);
		CHECK(derived2_rtti_instance != nullptr);
		CRTTIMethod* method2 = derived2_rtti_instance->FindMethodByName(method_name);
		CHECK(method2 != nullptr);
		const char* multiply_method_name = "Multiply";
		CRTTIMethod* multiply_method = derived2_rtti_instance->FindMethodByName(multiply_method_name);
		CHECK(multiply_method != nullptr);

		const char* derived_both_name = "pulvis::rtti::tests::CDerivedBoth";
		CRTTIClass* derived_both_rtti_instance = CRTTIClass::FindInStorage(derived_both_name);
		CHECK(derived_both_rtti_instance != nullptr);
		CRTTIMethod* method_both = derived_both_rtti_instance->FindMethodByName(method_name);
		CHECK(method_both != nullptr);
		CRTTIMethod* add_method_both = derived_both_rtti_instance->FindMethodByName(add_method_name);
		CRTTIMethod* multiply_method_both = derived_both_rtti_instance->FindMethodByName(multiply_method_name);
		CHECK(add_method_both != nullptr);
		CHECK(multiply_method_both != nullptr);
	}

	SECTION("Method calling works?")
	{
		const char* virtual_method_name = "GetVirtualBase";

		const char* derived_name = "pulvis::rtti::tests::CDerived";
		CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
		CHECK(derived_rtti_instance != nullptr);
		CRTTIMethod* method = derived_rtti_instance->FindMethodByName(virtual_method_name);
		CHECK(method != nullptr);
		CDerived derived;
		const int result = method->Invoke<int>(&derived);
		CHECK(result == 2);
		const int add_result = derived_rtti_instance->FindMethodByName("Add")->Invoke<int>(&derived, 1, 2);
		CHECK(add_result == 3);

		const char* derived2_name = "pulvis::rtti::tests::CDerived2";
		CRTTIClass* derived2_rtti_instance = CRTTIClass::FindInStorage(derived2_name);
		CHECK(derived2_rtti_instance != nullptr);
		CRTTIMethod* method2 = derived2_rtti_instance->FindMethodByName(virtual_method_name);
		CHECK(method2 != nullptr);
		CDerived2 derived2;
		const int result2 = method2->Invoke<int>(&derived2);
		CHECK(result2 == 3);
		const int multiply_result = derived2_rtti_instance->FindMethodByName("Multiply")->Invoke<int>(&derived2, 3, 4);
		CHECK(multiply_result == 12);
	}

#ifdef RUN_BENCHMARK
	SECTION("Performance tests")
	{
		const char* derived_name = "pulvis::rtti::tests::CDerived";
		const char* virtual_method_name = "GetVirtualBase";
		const char* add_method_name = "Add";

		BENCHMARK("Virtual method call (CRTTIMethod)")
		{
			CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
			CRTTIMethod* method = derived_rtti_instance->FindMethodByName(virtual_method_name);
			CDerived derived;
			return method->Invoke<int>(&derived);
		};

		BENCHMARK("Virtual method call (direct)")
		{
			CDerived derived;
			return derived.GetVirtualBase();
		};

		BENCHMARK("Non-virtual method call (CRTTIMethod)")
		{
			CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
			CRTTIMethod* method = derived_rtti_instance->FindMethodByName(add_method_name);
			CDerived derived;
			return method->Invoke<int>(&derived, 1, 2);
		};

		BENCHMARK("Non-virtual method call (direct)")
		{
			CDerived derived;
			return derived.Add(1, 2);
		};
	}
#endif
}

TEST_CASE("Field", "[RTTI][Field]")
{
	SECTION("Finding fields works?")
	{
		CRTTIField* derived_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerived")->FindFieldByName("IntField");
		CHECK(derived_int_field != nullptr);

		CRTTIField* derived2_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerived2")->FindFieldByName("IntField");
		CHECK(derived2_int_field != nullptr);

		CRTTIField* derived_both_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerivedBoth")->FindFieldByName("IntField");
		CHECK(derived_both_int_field != nullptr);
	}

	SECTION("Set / get fields works?")
	{
		CDerived derived;
		CRTTIField* derived_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerived")->FindFieldByName("IntField");
		CHECK(derived_int_field != nullptr);
		CHECK(derived_int_field->Get<int>(&derived) == 0);
		derived_int_field->Set(&derived, 42);
		CHECK(derived_int_field->Get<int>(&derived) == 42);

		CDerived2 derived2;
		CRTTIField* derived2_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerived2")->FindFieldByName("IntField");
		CHECK(derived2_int_field != nullptr);
		CHECK(derived2_int_field->Get<int>(&derived2) == 0);
		derived2_int_field->Set(&derived2, 24);
		CHECK(derived2_int_field->Get<int>(&derived2) == 24);

		CDerivedBoth derived_both;
		CRTTIField* derived_both_int_field = CRTTIClass::FindInStorage("pulvis::rtti::tests::CDerivedBoth")->FindFieldByName("IntField");
		CHECK(derived_both_int_field != nullptr);
		CHECK(derived_both_int_field->Get<int>(&derived_both) == 0);
		derived_both_int_field->Set(&derived_both, 123);
		CHECK(derived_both_int_field->Get<int>(&derived_both) == 123);
	}

#ifdef RUN_BENCHMARK
	SECTION("Performance tests")
	{
		const char* derived_name = "pulvis::rtti::tests::CDerived";
		const char* field_name = "IntField";

		BENCHMARK("Field CRTTIField)")
		{
			CRTTIClass* derived_rtti_instance = CRTTIClass::FindInStorage(derived_name);
			CRTTIField* field = derived_rtti_instance->FindFieldByName(field_name);
			CDerived derived;
			field->Set(&derived, 42);
			return field->Get<int>(&derived);
		};

		BENCHMARK("Field (direct)")
		{
			CDerived derived;
			derived.IntField = 42;
			return derived.IntField;
		};
	}
#endif
}