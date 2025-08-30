#include <vendor/common/include/catch2/catch2.hpp>

#include "tests/rtti/TestsRTTIMock_rtti_autogen.hpp"

void RegisterRTTI()
{
	AutoGenRtti_RegisterEnum_EMocRttiEnumExplicit();
	AutoGenRtti_RegisterEnum_EMocRttiEnumImplicit();
	AutoGenRtti_RegisterEnum_EMocRttiLongValuesNames();
	AutoGenRtti_RegisterClass_IBase();
	AutoGenRtti_RegisterClass_IBase2();
	AutoGenRtti_RegisterClass_CDerived();
	AutoGenRtti_RegisterClass_CDerived2();
	AutoGenRtti_RegisterClass_CDerivedBoth();
}

int main(int argc, const char* argv[])
{
	RegisterRTTI();
	return Catch::Session().run(argc, argv);
}
