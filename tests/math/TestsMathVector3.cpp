#include "engine/engine_pch.hpp"

#include "engine/math/Vector3.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::math;

TEST_CASE("Vector3", "[Math]")
{
	SECTION("Vector3 - constructors")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		REQUIRE(v_int[0] == 10);
		REQUIRE(v_int[1] == 20);
		REQUIRE(v_int[2] == 30);
		Vector3<float> v_float{ 10.0f, 20.0f, 30.f };
		REQUIRE(v_float[0] == 10.0f);
		REQUIRE(v_float[1] == 20.0f);
		REQUIRE(v_float[2] == 30.0f);
		Vector3<double> v_double{ 10.0, 20.0, 30.0 };
		REQUIRE(v_double[0] == 10.0);
		REQUIRE(v_double[1] == 20.0);
		REQUIRE(v_double[2] == 30.0);
		Vector3<long> v_long{ 10, 20, 30 };
		REQUIRE(v_long[0] == 10);
		REQUIRE(v_long[1] == 20);
		REQUIRE(v_long[2] == 30);
		Vector3<long long> v_long_long{ 10, 20, 30 };
		REQUIRE(v_long_long[0] == 10);
		REQUIRE(v_long_long[1] == 20);
		REQUIRE(v_long_long[2] == 30);
		Vector3<short> v_short{ 10, 20, 30 };
		REQUIRE(v_short[0] == 10);
		REQUIRE(v_short[1] == 20);
		REQUIRE(v_short[2] == 30);
		Vector3<char> v_char{ 10, 20, 30 };
		REQUIRE(v_char[0] == 10);
		REQUIRE(v_char[1] == 20);
		REQUIRE(v_char[2] == 30);
		Vector3<unsigned int> v_uint{ 10, 20, 30 };
		REQUIRE(v_uint[0] == 10);
		REQUIRE(v_uint[1] == 20);
		REQUIRE(v_uint[2] == 30);
		Vector3<unsigned char> v_uchar{ 10, 20, 30 };
		REQUIRE(v_uchar[0] == 10);
		REQUIRE(v_uchar[1] == 20);
		REQUIRE(v_uchar[2] == 30);
		Vector3<unsigned short> v_ushort{ 10, 20, 30 };
		REQUIRE(v_ushort[0] == 10);
		REQUIRE(v_ushort[1] == 20);
		REQUIRE(v_ushort[2] == 30);
		Vector3<unsigned long> v_ulong{ 10, 20, 30 };
		REQUIRE(v_ulong[0] == 10);
		REQUIRE(v_ulong[1] == 20);
		REQUIRE(v_ulong[2] == 30);
		Vector3<unsigned long long> v_ulong_long{ 10, 20, 30 };
		REQUIRE(v_ulong_long[0] == 10);
		REQUIRE(v_ulong_long[1] == 20);
		REQUIRE(v_ulong_long[2] == 30);
	}

	SECTION("Vector3 - rule of 5")
	{

	}

	SECTION("Vector3 - methods")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		Vector3<int> v_int2{ 1, 2, 3 };
		REQUIRE(v_int.Length() == 37);
		REQUIRE(v_int.LengthSquared() == 1400);
		REQUIRE(v_int.Dot(v_int2) == 140);
		Vector3<int> v_int_cross = v_int.Cross(v_int2);
		REQUIRE(v_int_cross[0] == 0);
		REQUIRE(v_int_cross[1] == 0);
		REQUIRE(v_int_cross[2] == 0);
		REQUIRE(v_int.Normalize().Length() == 0);

		Vector3<float> v_float{ 10.0f, 20.0f, 30.0f };
		Vector3<float> v_float2{ 1.0f, 2.0f, 3.0f };
		REQUIRE(v_float.Length() == 37.416572571f);
		REQUIRE(v_float.LengthSquared() == 1400.0f);
		REQUIRE(v_float.Dot(v_float2) == 140.0f);
		Vector3<float> v_float_cross = v_float.Cross(v_float2);
		REQUIRE(v_float_cross[0] == 0.0f);
		REQUIRE(v_float_cross[1] == 0.0f);
		REQUIRE(v_float_cross[2] == 0.0f);
		REQUIRE(v_float.Normalize().Length() == Catch::Approx(1.0f));
	}

	SECTION("Vector3 - operators")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		Vector3<int> v_int2{ 1, 2, 3 };

		{
			const Vector3<int> v_int_add_res = v_int + v_int2;
			REQUIRE(v_int_add_res[0] == 11);
			REQUIRE(v_int_add_res[1] == 22);
			REQUIRE(v_int_add_res[2] == 33);
			const Vector3<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res[0] == 9);
			REQUIRE(v_int_sub_res[1] == 18);
			REQUIRE(v_int_sub_res[2] == 27);
			const Vector3<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res[0] == 10);
			REQUIRE(v_int_mul_res[1] == 40);
			REQUIRE(v_int_mul_res[2] == 90);
			const Vector3<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res[0] == 10);
			REQUIRE(v_int_div_res[1] == 10);
			REQUIRE(v_int_div_res[2] == 10);
		}

		{
			Vector3<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 22);
			REQUIRE(tmp[2] == 33);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 18);
			REQUIRE(tmp[2] == 27);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 40);
			REQUIRE(tmp[2] == 90);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 10);
			REQUIRE(tmp[2] == 10);
		}

		{
			Vector3<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 21);
			REQUIRE(tmp[2] == 31);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 19);
			REQUIRE(tmp[2] == 29);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp[0] == 20);
			REQUIRE(tmp[1] == 40);
			REQUIRE(tmp[2] == 60);
		}

		Vector3<float> v_float{ 10.0f, 20.0f, 30.0f };

	}

	SECTION("Vector3 - casts")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		Vector3<float> v_float{ 10.0f, 20.0f, 30.f };
		Vector3<double> v_double{ 10.0, 20.0, 30 };
		Vector3<long> v_long{ 10, 20, 30 };
		Vector3<long long> v_long_long{ 10, 20, 30 };
		Vector3<short> v_short{ 10, 20, 30 };
		Vector3<char> v_char{ 10, 20, 30 };
		Vector3<unsigned int> v_uint{ 10, 20, 30};
		Vector3<unsigned char> v_uchar{ 10, 20, 30 };
		Vector3<unsigned short> v_ushort{ 10, 20, 30 };
		Vector3<unsigned long> v_ulong{ 10, 20, 30 };
		Vector3<unsigned long long> v_ulong_long{ 10, 20, 30 };

		// Int to others
		{
			Vector3<float> v_float_cast = v_int.Cast<float>();
			REQUIRE(v_float_cast[0] == 10.0f);
			REQUIRE(v_float_cast[1] == 20.0f);
			REQUIRE(v_float_cast[2] == 30.0f);
			Vector3<double> v_double_cast = v_int.Cast<double>();
			REQUIRE(v_double_cast[0] == 10.0);
			REQUIRE(v_double_cast[1] == 20.0);
			REQUIRE(v_double_cast[2] == 30.0);
			Vector3<long> v_long_cast = v_int.Cast<long>();
			REQUIRE(v_long_cast[0] == 10);
			REQUIRE(v_long_cast[1] == 20);
			REQUIRE(v_long_cast[2] == 30);
			Vector3<long long> v_long_long_cast = v_int.Cast<long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			Vector3<short> v_short_cast = v_int.Cast<short>();
			REQUIRE(v_short_cast[0] == 10);
			REQUIRE(v_short_cast[1] == 20);
			REQUIRE(v_short_cast[2] == 30);
			Vector3<char> v_char_cast = v_int.Cast<char>();
			REQUIRE(v_char_cast[0] == 10);
			REQUIRE(v_char_cast[1] == 20);
			REQUIRE(v_char_cast[2] == 30);
			Vector3<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
			REQUIRE(v_uint_cast[0] == 10);
			REQUIRE(v_uint_cast[1] == 20);
			REQUIRE(v_uint_cast[2] == 30);
			Vector3<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
			REQUIRE(v_uchar_cast[0] == 10);
			REQUIRE(v_uchar_cast[1] == 20);
			REQUIRE(v_uchar_cast[2] == 30);
			Vector3<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
			REQUIRE(v_ushort_cast[0] == 10);
			REQUIRE(v_ushort_cast[1] == 20);
			REQUIRE(v_ushort_cast[2] == 30);
			Vector3<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
			REQUIRE(v_ulong_cast[0] == 10);
			REQUIRE(v_ulong_cast[1] == 20);
			REQUIRE(v_ulong_cast[2] == 30);
			Vector3<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
		}

		// Float to others
		{
			Vector3<int> v_int_cast = v_float.Cast<int>();
			REQUIRE(v_int_cast[0] == 10);
			REQUIRE(v_int_cast[1] == 20);
			REQUIRE(v_int_cast[2] == 30);
			Vector3<double> v_double_cast = v_float.Cast<double>();
			REQUIRE(v_double_cast[0] == 10.0);
			REQUIRE(v_double_cast[1] == 20.0);
			REQUIRE(v_double_cast[2] == 30.0);
			Vector3<long> v_long_cast = v_float.Cast<long>();
			REQUIRE(v_long_cast[0] == 10);
			REQUIRE(v_long_cast[1] == 20);
			REQUIRE(v_long_cast[2] == 30);
			Vector3<long long> v_long_long_cast = v_float.Cast<long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			Vector3<short> v_short_cast = v_float.Cast<short>();
			REQUIRE(v_short_cast[0] == 10);
			REQUIRE(v_short_cast[1] == 20);
			REQUIRE(v_short_cast[2] == 30);
			Vector3<char> v_char_cast = v_float.Cast<char>();
			REQUIRE(v_char_cast[0] == 10);
			REQUIRE(v_char_cast[1] == 20);
			REQUIRE(v_char_cast[2] == 30);
			Vector3<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
			REQUIRE(v_uint_cast[0] == 10);
			REQUIRE(v_uint_cast[1] == 20);
			REQUIRE(v_uint_cast[2] == 30);
			Vector3<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
			REQUIRE(v_uchar_cast[0] == 10);
			REQUIRE(v_uchar_cast[1] == 20);
			REQUIRE(v_uchar_cast[2] == 30);
			Vector3<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
			REQUIRE(v_ushort_cast[0] == 10);
			REQUIRE(v_ushort_cast[1] == 20);
			REQUIRE(v_ushort_cast[2] == 30);
			Vector3<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
			REQUIRE(v_ulong_cast[0] == 10);
			REQUIRE(v_ulong_cast[1] == 20);
			REQUIRE(v_ulong_cast[2] == 30);
			Vector3<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
		}
	}
}