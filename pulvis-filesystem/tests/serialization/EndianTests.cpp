#include "serialization/Endian.hpp"

#include <catch2/catch2.hpp>

#include <cstring>

using namespace pulvis::fs::serialization;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Endian (ByteSwap16)", "[FS][Serialization][Endian]")
{
	SECTION("Swap known value")
	{
		CHECK(ByteSwap16(0x0102) == 0x0201);
	}

	SECTION("Swap zero")
	{
		CHECK(ByteSwap16(0x0000) == 0x0000);
	}

	SECTION("Swap max value")
	{
		CHECK(ByteSwap16(0xFFFF) == 0xFFFF);
	}

	SECTION("Double swap returns original")
	{
		uint16_t original = 0x1234;
		uint16_t swapped = ByteSwap16(original);
		CHECK(ByteSwap16(swapped) == original);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Endian (ByteSwap32)", "[FS][Serialization][Endian]")
{
	SECTION("Swap known value")
	{
		CHECK(ByteSwap32(0x01020304) == 0x04030201);
	}

	SECTION("Swap zero")
	{
		CHECK(ByteSwap32(0x00000000) == 0x00000000);
	}

	SECTION("Swap max")
	{
		CHECK(ByteSwap32(0xFFFFFFFF) == 0xFFFFFFFF);
	}

	SECTION("Double swap is identity")
	{
		uint32_t original = 0xDEADBEEF;
		CHECK(ByteSwap32(ByteSwap32(original)) == original);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Endian (ByteSwap64)", "[FS][Serialization][Endian]")
{
	SECTION("Swap known value")
	{
		CHECK(ByteSwap64(0x0102030405060708ULL) == 0x0807060504030201ULL);
	}

	SECTION("Swap zero")
	{
		CHECK(ByteSwap64(0x0000000000000000ULL) == 0x0000000000000000ULL);
	}

	SECTION("Swap max")
	{
		CHECK(ByteSwap64(0xFFFFFFFFFFFFFFFFULL) == 0xFFFFFFFFFFFFFFFFULL);
	}

	SECTION("Double swap is identity")
	{
		uint64_t original = 0xCAFEBABEDEADC0DEULL;
		CHECK(ByteSwap64(ByteSwap64(original)) == original);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("Endian (ToLittleEndian / FromLittleEndian)", "[FS][Serialization][Endian]")
{
	SECTION("Roundtrip uint16_t")
	{
		uint16_t original = 0xABCD;
		uint16_t le = ToLittleEndian(original);
		uint16_t back = FromLittleEndian(le);
		CHECK(back == original);
	}

	SECTION("Roundtrip uint32_t")
	{
		uint32_t original = 0xDEADBEEF;
		uint32_t le = ToLittleEndian(original);
		uint32_t back = FromLittleEndian(le);
		CHECK(back == original);
	}

	SECTION("Roundtrip uint64_t")
	{
		uint64_t original = 0x0102030405060708ULL;
		uint64_t le = ToLittleEndian(original);
		uint64_t back = FromLittleEndian(le);
		CHECK(back == original);
	}

	SECTION("Roundtrip int32_t")
	{
		int32_t original = -123456;
		int32_t le = ToLittleEndian(original);
		int32_t back = FromLittleEndian(le);
		CHECK(back == original);
	}

	SECTION("Roundtrip double")
	{
		double original = 3.14159265358979;
		double le = ToLittleEndian(original);
		double back = FromLittleEndian(le);
		CHECK(back == original);
	}

	SECTION("Single byte is identity")
	{
		uint8_t original = 0x42;
		CHECK(ToLittleEndian(original) == original);
		CHECK(FromLittleEndian(original) == original);
	}

	SECTION("GetNativeEndian returns Little on x86")
	{
		CHECK(GetNativeEndian() == EEndian::Little);
	}
}
