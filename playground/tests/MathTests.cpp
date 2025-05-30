#include "engine/engine_pch.hpp"

#include "engine/math/Vector2.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/math/Vector4.hpp"
#include "engine/math/MathUtils.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

TEST_CASE("Utils", "[Math]")
{
	SECTION("Normalize Radians")
	{

	}

	SECTION("Radians to degerees")
	{

	}

	SECTION("Degrees to radians")
	{

	}
}

TEST_CASE("Vector2", "[Math]")
{
	using namespace engine::math;

	SECTION("Vector2 - constructors")
	{
		Vector2<int> v_int{ 10, 20 };
		REQUIRE(v_int[0] == 10);
		REQUIRE(v_int[1] == 20);
		Vector2<float> v_float{ 10.0f, 20.0f };
		REQUIRE(v_float[0] == 10.0f);
		REQUIRE(v_float[1] == 20.0f);
		Vector2<double> v_double{ 10.0, 20.0 };
		REQUIRE(v_double[0] == 10.0);
		REQUIRE(v_double[1] == 20.0);
		Vector2<long> v_long{ 10, 20 };
		REQUIRE(v_long[0] == 10);
		REQUIRE(v_long[1] == 20);
		Vector2<long long> v_long_long{ 10, 20 };
		REQUIRE(v_long_long[0] == 10);
		REQUIRE(v_long_long[1] == 20);
		Vector2<short> v_short{ 10, 20 };
		REQUIRE(v_short[0] == 10);
		REQUIRE(v_short[1] == 20);
		Vector2<char> v_char{ 10, 20 };
		REQUIRE(v_char[0] == 10);
		REQUIRE(v_char[1] == 20);
		Vector2<unsigned int> v_uint{ 10, 20 };
		REQUIRE(v_uint[0] == 10);
		REQUIRE(v_uint[1] == 20);
		Vector2<unsigned char> v_uchar{ 10, 20 };
		REQUIRE(v_uchar[0] == 10);
		REQUIRE(v_uchar[1] == 20);
		Vector2<unsigned short> v_ushort{ 10, 20 };
		REQUIRE(v_ushort[0] == 10);
		REQUIRE(v_ushort[1] == 20);
		Vector2<unsigned long> v_ulong{ 10, 20 };
		REQUIRE(v_ulong[0] == 10);
		REQUIRE(v_ulong[1] == 20);
		Vector2<unsigned long long> v_ulong_long{ 10, 20 };
		REQUIRE(v_ulong_long[0] == 10);
		REQUIRE(v_ulong_long[1] == 20);
	}

	SECTION("Vector2 -- rule of 5")
	{

	}

	SECTION("Vector2 - methods")
	{
		Vector2<int> v_int{ 10, 20 };
		Vector2<int> v_int2{ 1, 2 };
		REQUIRE(v_int.Length() == 22);
		REQUIRE(v_int.LengthSquared() == 500);
		REQUIRE(v_int.Dot(v_int2) == 50);
		REQUIRE(v_int.Cross(v_int2) == 0);
		REQUIRE(v_int.Normalize().Length() == 0);

		Vector2<float> v_float{ 10.0f, 20.0f };
		Vector2<float> v_float2{ 1.0f, 2.0f };
		REQUIRE(v_float.Length() == 22.360679775f);
		REQUIRE(v_float.LengthSquared() == 500.0f);
		REQUIRE(v_float.Dot(v_float2) == 50.0f);
		REQUIRE(v_float.Cross(v_float2) == 0.f);
		REQUIRE(v_float.Normalize().Length() == Catch::Approx(1.0f));
	}

	SECTION("Vector2 - operators")
	{
		Vector2<int> v_int{ 10, 20 };
		Vector2<int> v_int2{ 1, 2 };

		{
			const Vector2<int> v_int_add_res = v_int + v_int2;
			REQUIRE(v_int_add_res[0] == 11);
			REQUIRE(v_int_add_res[1] == 22);
			const Vector2<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res[0] == 9);
			REQUIRE(v_int_sub_res[1] == 18);
			const Vector2<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res[0] == 10);
			REQUIRE(v_int_mul_res[1] == 40);
			const Vector2<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res[0] == 10);
			REQUIRE(v_int_div_res[1] == 10);
		}

		{
			Vector2<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 22);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 18);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 40);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 10);
		}

		{
			Vector2<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 21);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 19);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp[0] == 20);
			REQUIRE(tmp[1] == 40);
			tmp = v_int;
			tmp /= 2;
			REQUIRE(tmp[0] == 5);
			REQUIRE(tmp[1] == 10);
		}


		Vector2<float> v_float{ 10.0f, 20.0f };
		Vector2<float> v_float2{ 1.0f, 2.0f };

		{
			const Vector2<float> v_float_add_res = v_float + v_float2;
			REQUIRE(v_float_add_res[0] == 11.0f);
			REQUIRE(v_float_add_res[1] == 22.0f);
			const Vector2<float> v_float_sub_res = v_float - v_float2;
			REQUIRE(v_float_sub_res[0] == 9.0f);
			REQUIRE(v_float_sub_res[1] == 18.0f);
			const Vector2<float> v_float_mul_res = v_float * v_float2;
			REQUIRE(v_float_mul_res[0] == 10.0f);
			REQUIRE(v_float_mul_res[1] == 40.0f);
			const Vector2<float> v_float_div_res = v_float / v_float2;
			REQUIRE(v_float_div_res[0] == 10.0f);
			REQUIRE(v_float_div_res[1] == 10.0f);
		}

		{
			Vector2<float> tmp = v_float;
			tmp += v_float2;
			REQUIRE(tmp[0] == 11.0f);
			REQUIRE(tmp[1] == 22.0f);
			tmp = v_float;
			tmp -= v_float2;
			REQUIRE(tmp[0] == 9.0f);
			REQUIRE(tmp[1] == 18.0f);
			tmp = v_float;
			tmp *= v_float2;
			REQUIRE(tmp[0] == 10.0f);
			REQUIRE(tmp[1] == 40.0f);
			tmp = v_float;
			tmp /= v_float2;
			REQUIRE(tmp[0] == 10.0f);
			REQUIRE(tmp[1] == 10.0f);
		}

		{
			Vector2<float> tmp = v_float;
			tmp += 1.0f;
			REQUIRE(tmp[0] == 11.0f);
			REQUIRE(tmp[1] == 21.0f);
			tmp = v_float;
			tmp -= 1.0f;
			REQUIRE(tmp[0] == 9.0f);
			REQUIRE(tmp[1] == 19.0f);
			tmp = v_float;
			tmp *= 2.0f;
			REQUIRE(tmp[0] == 20.0f);
			REQUIRE(tmp[1] == 40.0f);
			tmp = v_float;
			tmp /= 2.0f;
			REQUIRE(tmp[0] == 5.0f);
			REQUIRE(tmp[1] == 10.0f);
		}
	}

		SECTION("Vector2 - Casts")
		{
			Vector2<int> v_int{ 10, 20 };
			Vector2<float> v_float{ 10.0f, 20.0f };
			Vector2<double> v_double{ 10.0, 20.0 };
			Vector2<long> v_long{ 10, 20 };
			Vector2<long long> v_long_long{ 10, 20 };
			Vector2<short> v_short{ 10, 20 };
			Vector2<char> v_char{ 10, 20 };
			Vector2<unsigned int> v_uint{ 10, 20 };
			Vector2<unsigned char> v_uchar{ 10, 20 };
			Vector2<unsigned short> v_ushort{ 10, 20 };
			Vector2<unsigned long> v_ulong{ 10, 20 };
			Vector2<unsigned long long> v_ulong_long{ 10, 20 };

			// Int to others
			{
				Vector2<float> v_float_cast = v_int.Cast<float>();
				REQUIRE(v_float_cast[0] == 10.0f);
				REQUIRE(v_float_cast[1] == 20.0f);
				Vector2<double> v_double_cast = v_int.Cast<double>();
				REQUIRE(v_double_cast[0] == 10.0);
				REQUIRE(v_double_cast[1] == 20.0);
				Vector2<long> v_long_cast = v_int.Cast<long>();
				REQUIRE(v_long_cast[0] == 10);
				REQUIRE(v_long_cast[1] == 20);
				Vector2<long long> v_long_long_cast = v_int.Cast<long long>();
				REQUIRE(v_long_long_cast[0] == 10);
				REQUIRE(v_long_long_cast[1] == 20);
				Vector2<short> v_short_cast = v_int.Cast<short>();
				REQUIRE(v_short_cast[0] == 10);
				REQUIRE(v_short_cast[1] == 20);
				Vector2<char> v_char_cast = v_int.Cast<char>();
				REQUIRE(v_char_cast[0] == 10);
				REQUIRE(v_char_cast[1] == 20);
				Vector2<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
				REQUIRE(v_uint_cast[0] == 10);
				REQUIRE(v_uint_cast[1] == 20);
				Vector2<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
				REQUIRE(v_uchar_cast[0] == 10);
				REQUIRE(v_uchar_cast[1] == 20);
				Vector2<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
				REQUIRE(v_ushort_cast[0] == 10);
				REQUIRE(v_ushort_cast[1] == 20);
				Vector2<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
				REQUIRE(v_ulong_cast[0] == 10);
				REQUIRE(v_ulong_cast[1] == 20);
				Vector2<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
			}

				// Float to others
			{
				Vector2<int> v_int_cast = v_float.Cast<int>();
				REQUIRE(v_int_cast[0] == 10);
				REQUIRE(v_int_cast[1] == 20);
				Vector2<double> v_double_cast = v_float.Cast<double>();
				REQUIRE(v_double_cast[0] == 10.0);
				REQUIRE(v_double_cast[1] == 20.0);
				Vector2<long> v_long_cast = v_float.Cast<long>();
				REQUIRE(v_long_cast[0] == 10);
				REQUIRE(v_long_cast[1] == 20);
				Vector2<long long> v_long_long_cast = v_float.Cast<long long>();
				REQUIRE(v_long_long_cast[0] == 10);
				REQUIRE(v_long_long_cast[1] == 20);
				Vector2<short> v_short_cast = v_float.Cast<short>();
				REQUIRE(v_short_cast[0] == 10);
				REQUIRE(v_short_cast[1] == 20);
				Vector2<char> v_char_cast = v_float.Cast<char>();
				REQUIRE(v_char_cast[0] == 10);
				REQUIRE(v_char_cast[1] == 20);
				Vector2<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
				REQUIRE(v_uint_cast[0] == 10);
				REQUIRE(v_uint_cast[1] == 20);
				Vector2<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
				REQUIRE(v_uchar_cast[0] == 10);
				REQUIRE(v_uchar_cast[1] == 20);
				Vector2<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
				REQUIRE(v_ushort_cast[0] == 10);
				REQUIRE(v_ushort_cast[1] == 20);
				Vector2<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
				REQUIRE(v_ulong_cast[0] == 10);
				REQUIRE(v_ulong_cast[1] == 20);
				Vector2<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
				REQUIRE(v_long_long_cast[0] == 10);
				REQUIRE(v_long_long_cast[1] == 20);
			}
	}
}

TEST_CASE("Vector3", "[Math]")
{
	using namespace engine::math;

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

TEST_CASE("Vector4", "[Math]")
{
	using namespace engine::math;

	SECTION("Vector4 - constructors")
	{
		Vector4<int> v_int{ 10, 20, 30, 40 };
		REQUIRE(v_int[0] == 10);
		REQUIRE(v_int[1] == 20);
		REQUIRE(v_int[2] == 30);
		REQUIRE(v_int[3] == 40);
		Vector4<float> v_float{ 10.0f, 20.0f, 30.f, 40.f };
		REQUIRE(v_float[0] == 10.0f);
		REQUIRE(v_float[1] == 20.0f);
		REQUIRE(v_float[2] == 30.0f);
		REQUIRE(v_float[3] == 40.0f);
		Vector4<double> v_double{ 10.0, 20.0, 30.0, 40.0 };
		REQUIRE(v_double[0] == 10.0);
		REQUIRE(v_double[1] == 20.0);
		REQUIRE(v_double[2] == 30.0);
		REQUIRE(v_double[3] == 40.0);
		Vector4<long> v_long{ 10, 20, 30, 40 };
		REQUIRE(v_long[0] == 10);
		REQUIRE(v_long[1] == 20);
		REQUIRE(v_long[2] == 30);
		REQUIRE(v_long[3] == 40);
		Vector4<long long> v_long_long{ 10, 20, 30, 40 };
		REQUIRE(v_long_long[0] == 10);
		REQUIRE(v_long_long[1] == 20);
		REQUIRE(v_long_long[2] == 30);
		REQUIRE(v_long_long[3] == 40);
		Vector4<short> v_short{ 10, 20, 30, 40 };
		REQUIRE(v_short[0] == 10);
		REQUIRE(v_short[1] == 20);
		REQUIRE(v_short[2] == 30);
		REQUIRE(v_short[3] == 40);
		Vector4<char> v_char{ 10, 20, 30, 40 };
		REQUIRE(v_char[0] == 10);
		REQUIRE(v_char[1] == 20);
		REQUIRE(v_char[2] == 30);
		REQUIRE(v_char[3] == 40);
		Vector4<unsigned int> v_uint{ 10, 20, 30, 40 };
		REQUIRE(v_uint[0] == 10);
		REQUIRE(v_uint[1] == 20);
		REQUIRE(v_uint[2] == 30);
		REQUIRE(v_uint[3] == 40);
		Vector4<unsigned char> v_uchar{ 10, 20, 30, 40 };
		REQUIRE(v_uchar[0] == 10);
		REQUIRE(v_uchar[1] == 20);
		REQUIRE(v_uchar[2] == 30);
		REQUIRE(v_uchar[3] == 40);
		Vector4<unsigned short> v_ushort{ 10, 20, 30, 40 };
		REQUIRE(v_ushort[0] == 10);
		REQUIRE(v_ushort[1] == 20);
		REQUIRE(v_ushort[2] == 30);
		REQUIRE(v_ushort[3] == 40);
		Vector4<unsigned long> v_ulong{ 10, 20, 30, 40 };
		REQUIRE(v_ulong[0] == 10);
		REQUIRE(v_ulong[1] == 20);
		REQUIRE(v_ulong[2] == 30);
		REQUIRE(v_ulong[3] == 40);
		Vector4<unsigned long long> v_ulong_long{ 10, 20, 30, 40 };
		REQUIRE(v_ulong_long[0] == 10);
		REQUIRE(v_ulong_long[1] == 20);
		REQUIRE(v_ulong_long[2] == 30);
		REQUIRE(v_ulong_long[3] == 40);
	}

	SECTION("Vector4 - rule of 5")
	{

	}

	SECTION("Vector4 - methods")
	{
		Vector4<int> v_int{ 10, 20, 30, 40 };
		Vector4<int> v_int2{ 1, 2, 3, 4 };
		REQUIRE(v_int.Length() == 54);
		REQUIRE(v_int.LengthSquared() == 3'000);
		REQUIRE(v_int.Dot(v_int2) == 300);
		Vector4<int> v_int_cross = v_int.Cross(v_int2);
		REQUIRE(v_int_cross[0] == 0);
		REQUIRE(v_int_cross[1] == 0);
		REQUIRE(v_int_cross[2] == 0);
		REQUIRE(v_int_cross[3] == 0);
		REQUIRE(v_int.Normalize().Length() == 0);

		Vector4<float> v_float{ 10.0f, 20.0f, 30.0f, 40.f };
		Vector4<float> v_float2{ 1.0f, 2.0f, 3.0f, 4.f };
		REQUIRE(v_float.Length() == 54.772254944f);
		REQUIRE(v_float.LengthSquared() == 3000.0f);
		REQUIRE(v_float.Dot(v_float2) == 300.0f);
		Vector4<float> v_float_cross = v_float.Cross(v_float2);
		REQUIRE(v_float_cross[0] == 0.0f);
		REQUIRE(v_float_cross[1] == 0.0f);
		REQUIRE(v_float_cross[2] == 0.0f);
		REQUIRE(v_float_cross[3] == 0.0f);
		REQUIRE(v_float.Normalize().Length() == Catch::Approx(1.0f).margin(1.e-2));
	}

	SECTION("Vector4 - operators")
	{
		Vector4<int> v_int{ 10, 20, 30 , 40};
		Vector4<int> v_int2{ 1, 2, 3, 4 };

		{
			const Vector4<int> v_int_add_res = v_int + v_int2;
			REQUIRE(v_int_add_res[0] == 11);
			REQUIRE(v_int_add_res[1] == 22);
			REQUIRE(v_int_add_res[2] == 33);
			REQUIRE(v_int_add_res[3] == 44);
			const Vector4<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res[0] == 9);
			REQUIRE(v_int_sub_res[1] == 18);
			REQUIRE(v_int_sub_res[2] == 27);
			REQUIRE(v_int_sub_res[3] == 36);
			const Vector4<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res[0] == 10);
			REQUIRE(v_int_mul_res[1] == 40);
			REQUIRE(v_int_mul_res[2] == 90);
			REQUIRE(v_int_mul_res[3] == 160);
			const Vector4<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res[0] == 10);
			REQUIRE(v_int_div_res[1] == 10);
			REQUIRE(v_int_div_res[2] == 10);
			REQUIRE(v_int_div_res[3] == 10);
		}

		{
			Vector4<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 22);
			REQUIRE(tmp[2] == 33);
			REQUIRE(tmp[3] == 44);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 18);
			REQUIRE(tmp[2] == 27);
			REQUIRE(tmp[3] == 36);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 40);
			REQUIRE(tmp[2] == 90);
			REQUIRE(tmp[3] == 160);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp[0] == 10);
			REQUIRE(tmp[1] == 10);
			REQUIRE(tmp[2] == 10);
			REQUIRE(tmp[3] == 10);
		}

		{
			Vector4<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp[0] == 11);
			REQUIRE(tmp[1] == 21);
			REQUIRE(tmp[2] == 31);
			REQUIRE(tmp[3] == 41);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp[0] == 9);
			REQUIRE(tmp[1] == 19);
			REQUIRE(tmp[2] == 29);
			REQUIRE(tmp[3] == 39);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp[0] == 20);
			REQUIRE(tmp[1] == 40);
			REQUIRE(tmp[2] == 60);
			REQUIRE(tmp[3] == 80);
		}
	}

	SECTION("Vector4 - casts")
	{
		Vector4<int> v_int{ 10, 20, 30, 40 };
		Vector4<float> v_float{ 10.0f, 20.0f, 30.f, 40.f };
		Vector4<double> v_double{ 10.0, 20.0, 30, 40 };
		Vector4<long> v_long{ 10, 20, 30, 40 };
		Vector4<long long> v_long_long{ 10, 20, 30, 40 };
		Vector4<short> v_short{ 10, 20, 30, 40 };
		Vector4<char> v_char{ 10, 20, 30, 40 };
		Vector4<unsigned int> v_uint{ 10, 20, 30, 40 };
		Vector4<unsigned char> v_uchar{ 10, 20, 30, 40 };
		Vector4<unsigned short> v_ushort{ 10, 20, 30, 40 };
		Vector4<unsigned long> v_ulong{ 10, 20, 30, 40 };
		Vector4<unsigned long long> v_ulong_long{ 10, 20, 30, 40 };

		// Int to others
		{
			Vector4<float> v_float_cast = v_int.Cast<float>();
			REQUIRE(v_float_cast[0] == 10.0f);
			REQUIRE(v_float_cast[1] == 20.0f);
			REQUIRE(v_float_cast[2] == 30.0f);
			REQUIRE(v_float_cast[3] == 40.0f);
			Vector4<double> v_double_cast = v_int.Cast<double>();
			REQUIRE(v_double_cast[0] == 10.0);
			REQUIRE(v_double_cast[1] == 20.0);
			REQUIRE(v_double_cast[2] == 30.0);
			REQUIRE(v_double_cast[3] == 40.0);
			Vector4<long> v_long_cast = v_int.Cast<long>();
			REQUIRE(v_long_cast[0] == 10);
			REQUIRE(v_long_cast[1] == 20);
			REQUIRE(v_long_cast[2] == 30);
			REQUIRE(v_long_cast[3] == 40);
			Vector4<long long> v_long_long_cast = v_int.Cast<long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			REQUIRE(v_long_long_cast[3] == 40);
			Vector4<short> v_short_cast = v_int.Cast<short>();
			REQUIRE(v_short_cast[0] == 10);
			REQUIRE(v_short_cast[1] == 20);
			REQUIRE(v_short_cast[2] == 30);
			REQUIRE(v_short_cast[3] == 40);
			Vector4<char> v_char_cast = v_int.Cast<char>();
			REQUIRE(v_char_cast[0] == 10);
			REQUIRE(v_char_cast[1] == 20);
			REQUIRE(v_char_cast[2] == 30);
			REQUIRE(v_char_cast[3] == 40);
			Vector4<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
			REQUIRE(v_uint_cast[0] == 10);
			REQUIRE(v_uint_cast[1] == 20);
			REQUIRE(v_uint_cast[2] == 30);
			REQUIRE(v_uint_cast[3] == 40);
			Vector4<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
			REQUIRE(v_uchar_cast[0] == 10);
			REQUIRE(v_uchar_cast[1] == 20);
			REQUIRE(v_uchar_cast[2] == 30);
			REQUIRE(v_uchar_cast[3] == 40);
			Vector4<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
			REQUIRE(v_ushort_cast[0] == 10);
			REQUIRE(v_ushort_cast[1] == 20);
			REQUIRE(v_ushort_cast[2] == 30);
			REQUIRE(v_ushort_cast[3] == 40);
			Vector4<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
			REQUIRE(v_ulong_cast[0] == 10);
			REQUIRE(v_ulong_cast[1] == 20);
			REQUIRE(v_ulong_cast[2] == 30);
			REQUIRE(v_ulong_cast[3] == 40);
			Vector4<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			REQUIRE(v_long_long_cast[3] == 40);
		}

		// Float to others
		{
			Vector4<int> v_int_cast = v_float.Cast<int>();
			REQUIRE(v_int_cast[0] == 10);
			REQUIRE(v_int_cast[1] == 20);
			REQUIRE(v_int_cast[2] == 30);
			REQUIRE(v_int_cast[3] == 40);
			Vector4<double> v_double_cast = v_float.Cast<double>();
			REQUIRE(v_double_cast[0] == 10.0);
			REQUIRE(v_double_cast[1] == 20.0);
			REQUIRE(v_double_cast[2] == 30.0);
			REQUIRE(v_double_cast[3] == 40.0);
			Vector4<long> v_long_cast = v_float.Cast<long>();
			REQUIRE(v_long_cast[0] == 10);
			REQUIRE(v_long_cast[1] == 20);
			REQUIRE(v_long_cast[2] == 30);
			REQUIRE(v_long_cast[3] == 40);
			Vector4<long long> v_long_long_cast = v_float.Cast<long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			REQUIRE(v_long_long_cast[3] == 40);
			Vector4<short> v_short_cast = v_float.Cast<short>();
			REQUIRE(v_short_cast[0] == 10);
			REQUIRE(v_short_cast[1] == 20);
			REQUIRE(v_short_cast[2] == 30);
			REQUIRE(v_short_cast[3] == 40);
			Vector4<char> v_char_cast = v_float.Cast<char>();
			REQUIRE(v_char_cast[0] == 10);
			REQUIRE(v_char_cast[1] == 20);
			REQUIRE(v_char_cast[2] == 30);
			REQUIRE(v_char_cast[3] == 40);
			Vector4<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
			REQUIRE(v_uint_cast[0] == 10);
			REQUIRE(v_uint_cast[1] == 20);
			REQUIRE(v_uint_cast[2] == 30);
			REQUIRE(v_uint_cast[3] == 40);
			Vector4<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
			REQUIRE(v_uchar_cast[0] == 10);
			REQUIRE(v_uchar_cast[1] == 20);
			REQUIRE(v_uchar_cast[2] == 30);
			REQUIRE(v_uchar_cast[3] == 40);
			Vector4<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
			REQUIRE(v_ushort_cast[0] == 10);
			REQUIRE(v_ushort_cast[1] == 20);
			REQUIRE(v_ushort_cast[2] == 30);
			REQUIRE(v_ushort_cast[3] == 40);
			Vector4<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
			REQUIRE(v_ulong_cast[0] == 10);
			REQUIRE(v_ulong_cast[1] == 20);
			REQUIRE(v_ulong_cast[2] == 30);
			REQUIRE(v_ulong_cast[3] == 40);
			Vector4<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast[0] == 10);
			REQUIRE(v_long_long_cast[1] == 20);
			REQUIRE(v_long_long_cast[2] == 30);
			REQUIRE(v_long_long_cast[3] == 40);
		}
	}
}