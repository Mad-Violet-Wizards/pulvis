#include "engine/engine_pch.hpp"

#include "engine/math/Vector2.hpp"
#include "engine/math/Vector3.hpp"
#include "engine/math/Vector4.hpp"
#include "engine/math/MathUtils.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

TEST_CASE("Utils", "[Math]")
{
	SECTION("Pow")
	{
		REQUIRE(engine::math::Pow<int>(0, 0) == 1);
		REQUIRE(engine::math::Pow<int>(0, 1) == 0);
		REQUIRE(engine::math::Pow<int>(1, 0) == 1);
		REQUIRE(engine::math::Pow<int>(1, 1) == 1);
		REQUIRE(engine::math::Pow<int>(2, 0) == 1);
		REQUIRE(engine::math::Pow<int>(2, 1) == 2);
		REQUIRE(engine::math::Pow<int>(2, 2) == 4);
		REQUIRE(engine::math::Pow<int>(2, 3) == 8);
		REQUIRE(engine::math::Pow<int>(2, 4) == 16);
		REQUIRE(engine::math::Pow<int>(2, 5) == 32);
		REQUIRE(engine::math::Pow<int>(2, 6) == 64);
		
		REQUIRE(engine::math::Pow<float>(0.0f, 0) == 1.0f);
		REQUIRE(engine::math::Pow<float>(0.0f, 1) == 0.0f);
		REQUIRE(engine::math::Pow<float>(1.0f, 0) == 1.0f);
		REQUIRE(engine::math::Pow<float>(1.0f, 1) == 1.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 0) == 1.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 1) == 2.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 2) == 4.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 3) == 8.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 4) == 16.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 5) == 32.0f);
		REQUIRE(engine::math::Pow<float>(2.0f, 6) == 64.0f);
	}

	SECTION("Abs")
	{
		REQUIRE(engine::math::Abs<int>(0) == 0);
		REQUIRE(engine::math::Abs<int>(-1) == 1);
		REQUIRE(engine::math::Abs<int>(1) == 1);
		REQUIRE(engine::math::Abs<int>(-2) == 2);
		REQUIRE(engine::math::Abs<int>(2) == 2);
		REQUIRE(engine::math::Abs<int>(-3) == 3);
		REQUIRE(engine::math::Abs<int>(3) == 3);

		REQUIRE(engine::math::Abs<float>(0.0f) == 0.0f);
		REQUIRE(engine::math::Abs<float>(-1.0f) == 1.0f);
		REQUIRE(engine::math::Abs<float>(1.0f) == 1.0f);
		REQUIRE(engine::math::Abs<float>(-2.0f) == 2.0f);
		REQUIRE(engine::math::Abs<float>(2.0f) == 2.0f);
		REQUIRE(engine::math::Abs<float>(-3.0f) == 3.0f);
		REQUIRE(engine::math::Abs<float>(3.0f) == 3.0f);
	}

	SECTION("Sqrt")
	{
		REQUIRE(engine::math::Sqrt<int>(0) == 0);
		REQUIRE(engine::math::Sqrt<int>(1) == 1);
		REQUIRE(engine::math::Sqrt<int>(2) == 1);
		REQUIRE(engine::math::Sqrt<int>(3) == 1);
		REQUIRE(engine::math::Sqrt<int>(4) == 2);
		REQUIRE(engine::math::Sqrt<int>(5) == 2);

		REQUIRE(engine::math::Sqrt<float>(0.0f, 8) == 0.0f);
		REQUIRE(engine::math::Sqrt<float>(1.0f, 8) == 1.0f);
		REQUIRE(engine::math::Sqrt<float>(2.0f, 8) == 1.41421356f);
		REQUIRE(engine::math::Sqrt<float>(3.0f, 8) == 1.73205081f);
		REQUIRE(engine::math::Sqrt<float>(4.0f, 8) == 2.0f);
	}
}

TEST_CASE("Vector2", "[Math]")
{
	using namespace engine::math;

	SECTION("Vector2 - constructors")
	{
		Vector2<int> v_int{ 10, 20 };
		REQUIRE(v_int.m_X == 10);
		REQUIRE(v_int.m_Y == 20);
		Vector2<float> v_float{ 10.0f, 20.0f };
		REQUIRE(v_float.m_X == 10.0f);
		REQUIRE(v_float.m_Y == 20.0f);
		Vector2<double> v_double{ 10.0, 20.0 };
		REQUIRE(v_double.m_X == 10.0);
		REQUIRE(v_double.m_Y == 20.0);
		Vector2<long> v_long{ 10, 20 };
		REQUIRE(v_long.m_X == 10);
		REQUIRE(v_long.m_Y == 20);
		Vector2<long long> v_long_long{ 10, 20 };
		REQUIRE(v_long_long.m_X == 10);
		REQUIRE(v_long_long.m_Y == 20);
		Vector2<short> v_short{ 10, 20 };
		REQUIRE(v_short.m_X == 10);
		REQUIRE(v_short.m_Y == 20);
		Vector2<char> v_char{ 10, 20 };
		REQUIRE(v_char.m_X == 10);
		REQUIRE(v_char.m_Y == 20);
		Vector2<unsigned int> v_uint{ 10, 20 };
		REQUIRE(v_uint.m_X == 10);
		REQUIRE(v_uint.m_Y == 20);
		Vector2<unsigned char> v_uchar{ 10, 20 };
		REQUIRE(v_uchar.m_X == 10);
		REQUIRE(v_uchar.m_Y == 20);
		Vector2<unsigned short> v_ushort{ 10, 20 };
		REQUIRE(v_ushort.m_X == 10);
		REQUIRE(v_ushort.m_Y == 20);
		Vector2<unsigned long> v_ulong{ 10, 20 };
		REQUIRE(v_ulong.m_X == 10);
		REQUIRE(v_ulong.m_Y == 20);
		Vector2<unsigned long long> v_ulong_long{ 10, 20 };
		REQUIRE(v_ulong_long.m_X == 10);
		REQUIRE(v_ulong_long.m_Y == 20);
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
			REQUIRE(v_int_add_res.m_X == 11);
			REQUIRE(v_int_add_res.m_Y == 22);
			const Vector2<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res.m_X == 9);
			REQUIRE(v_int_sub_res.m_Y == 18);
			const Vector2<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res.m_X == 10);
			REQUIRE(v_int_mul_res.m_Y == 40);
			const Vector2<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res.m_X == 10);
			REQUIRE(v_int_div_res.m_Y == 10);
		}

		{
			Vector2<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 22);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 18);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 40);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 10);
		}

		{
			Vector2<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 21);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 19);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp.m_X == 20);
			REQUIRE(tmp.m_Y == 40);
			tmp = v_int;
			tmp /= 2;
			REQUIRE(tmp.m_X == 5);
			REQUIRE(tmp.m_Y == 10);
		}


		Vector2<float> v_float{ 10.0f, 20.0f };
		Vector2<float> v_float2{ 1.0f, 2.0f };

		{
			const Vector2<float> v_float_add_res = v_float + v_float2;
			REQUIRE(v_float_add_res.m_X == 11.0f);
			REQUIRE(v_float_add_res.m_Y == 22.0f);
			const Vector2<float> v_float_sub_res = v_float - v_float2;
			REQUIRE(v_float_sub_res.m_X == 9.0f);
			REQUIRE(v_float_sub_res.m_Y == 18.0f);
			const Vector2<float> v_float_mul_res = v_float * v_float2;
			REQUIRE(v_float_mul_res.m_X == 10.0f);
			REQUIRE(v_float_mul_res.m_Y == 40.0f);
			const Vector2<float> v_float_div_res = v_float / v_float2;
			REQUIRE(v_float_div_res.m_X == 10.0f);
			REQUIRE(v_float_div_res.m_Y == 10.0f);
		}

		{
			Vector2<float> tmp = v_float;
			tmp += v_float2;
			REQUIRE(tmp.m_X == 11.0f);
			REQUIRE(tmp.m_Y == 22.0f);
			tmp = v_float;
			tmp -= v_float2;
			REQUIRE(tmp.m_X == 9.0f);
			REQUIRE(tmp.m_Y == 18.0f);
			tmp = v_float;
			tmp *= v_float2;
			REQUIRE(tmp.m_X == 10.0f);
			REQUIRE(tmp.m_Y == 40.0f);
			tmp = v_float;
			tmp /= v_float2;
			REQUIRE(tmp.m_X == 10.0f);
			REQUIRE(tmp.m_Y == 10.0f);
		}

		{
			Vector2<float> tmp = v_float;
			tmp += 1.0f;
			REQUIRE(tmp.m_X == 11.0f);
			REQUIRE(tmp.m_Y == 21.0f);
			tmp = v_float;
			tmp -= 1.0f;
			REQUIRE(tmp.m_X == 9.0f);
			REQUIRE(tmp.m_Y == 19.0f);
			tmp = v_float;
			tmp *= 2.0f;
			REQUIRE(tmp.m_X == 20.0f);
			REQUIRE(tmp.m_Y == 40.0f);
			tmp = v_float;
			tmp /= 2.0f;
			REQUIRE(tmp.m_X == 5.0f);
			REQUIRE(tmp.m_Y == 10.0f);
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
				REQUIRE(v_float_cast.m_X == 10.0f);
				REQUIRE(v_float_cast.m_Y == 20.0f);
				Vector2<double> v_double_cast = v_int.Cast<double>();
				REQUIRE(v_double_cast.m_X == 10.0);
				REQUIRE(v_double_cast.m_Y == 20.0);
				Vector2<long> v_long_cast = v_int.Cast<long>();
				REQUIRE(v_long_cast.m_X == 10);
				REQUIRE(v_long_cast.m_Y == 20);
				Vector2<long long> v_long_long_cast = v_int.Cast<long long>();
				REQUIRE(v_long_long_cast.m_X == 10);
				REQUIRE(v_long_long_cast.m_Y == 20);
				Vector2<short> v_short_cast = v_int.Cast<short>();
				REQUIRE(v_short_cast.m_X == 10);
				REQUIRE(v_short_cast.m_Y == 20);
				Vector2<char> v_char_cast = v_int.Cast<char>();
				REQUIRE(v_char_cast.m_X == 10);
				REQUIRE(v_char_cast.m_Y == 20);
				Vector2<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
				REQUIRE(v_uint_cast.m_X == 10);
				REQUIRE(v_uint_cast.m_Y == 20);
				Vector2<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
				REQUIRE(v_uchar_cast.m_X == 10);
				REQUIRE(v_uchar_cast.m_Y == 20);
				Vector2<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
				REQUIRE(v_ushort_cast.m_X == 10);
				REQUIRE(v_ushort_cast.m_Y == 20);
				Vector2<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
				REQUIRE(v_ulong_cast.m_X == 10);
				REQUIRE(v_ulong_cast.m_Y == 20);
				Vector2<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
			}

				// Float to others
			{
				Vector2<int> v_int_cast = v_float.Cast<int>();
				REQUIRE(v_int_cast.m_X == 10);
				REQUIRE(v_int_cast.m_Y == 20);
				Vector2<double> v_double_cast = v_float.Cast<double>();
				REQUIRE(v_double_cast.m_X == 10.0);
				REQUIRE(v_double_cast.m_Y == 20.0);
				Vector2<long> v_long_cast = v_float.Cast<long>();
				REQUIRE(v_long_cast.m_X == 10);
				REQUIRE(v_long_cast.m_Y == 20);
				Vector2<long long> v_long_long_cast = v_float.Cast<long long>();
				REQUIRE(v_long_long_cast.m_X == 10);
				REQUIRE(v_long_long_cast.m_Y == 20);
				Vector2<short> v_short_cast = v_float.Cast<short>();
				REQUIRE(v_short_cast.m_X == 10);
				REQUIRE(v_short_cast.m_Y == 20);
				Vector2<char> v_char_cast = v_float.Cast<char>();
				REQUIRE(v_char_cast.m_X == 10);
				REQUIRE(v_char_cast.m_Y == 20);
				Vector2<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
				REQUIRE(v_uint_cast.m_X == 10);
				REQUIRE(v_uint_cast.m_Y == 20);
				Vector2<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
				REQUIRE(v_uchar_cast.m_X == 10);
				REQUIRE(v_uchar_cast.m_Y == 20);
				Vector2<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
				REQUIRE(v_ushort_cast.m_X == 10);
				REQUIRE(v_ushort_cast.m_Y == 20);
				Vector2<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
				REQUIRE(v_ulong_cast.m_X == 10);
				REQUIRE(v_ulong_cast.m_Y == 20);
				Vector2<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
				REQUIRE(v_long_long_cast.m_X == 10);
				REQUIRE(v_long_long_cast.m_Y == 20);
			}
	}
}

TEST_CASE("Vector3", "[Math]")
{
	using namespace engine::math;

	SECTION("Vector3 - constructors")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		REQUIRE(v_int.m_X == 10);
		REQUIRE(v_int.m_Y == 20);
		REQUIRE(v_int.m_Z == 30);
		Vector3<float> v_float{ 10.0f, 20.0f, 30.f };
		REQUIRE(v_float.m_X == 10.0f);
		REQUIRE(v_float.m_Y == 20.0f);
		REQUIRE(v_float.m_Z == 30.0f);
		Vector3<double> v_double{ 10.0, 20.0, 30.0 };
		REQUIRE(v_double.m_X == 10.0);
		REQUIRE(v_double.m_Y == 20.0);
		REQUIRE(v_double.m_Z == 30.0);
		Vector3<long> v_long{ 10, 20, 30 };
		REQUIRE(v_long.m_X == 10);
		REQUIRE(v_long.m_Y == 20);
		REQUIRE(v_long.m_Z == 30);
		Vector3<long long> v_long_long{ 10, 20, 30 };
		REQUIRE(v_long_long.m_X == 10);
		REQUIRE(v_long_long.m_Y == 20);
		REQUIRE(v_long_long.m_Z == 30);
		Vector3<short> v_short{ 10, 20, 30 };
		REQUIRE(v_short.m_X == 10);
		REQUIRE(v_short.m_Y == 20);
		REQUIRE(v_short.m_Z == 30);
		Vector3<char> v_char{ 10, 20, 30 };
		REQUIRE(v_char.m_X == 10);
		REQUIRE(v_char.m_Y == 20);
		REQUIRE(v_char.m_Z == 30);
		Vector3<unsigned int> v_uint{ 10, 20, 30 };
		REQUIRE(v_uint.m_X == 10);
		REQUIRE(v_uint.m_Y == 20);
		REQUIRE(v_uint.m_Z == 30);
		Vector3<unsigned char> v_uchar{ 10, 20, 30 };
		REQUIRE(v_uchar.m_X == 10);
		REQUIRE(v_uchar.m_Y == 20);
		REQUIRE(v_uchar.m_Z == 30);
		Vector3<unsigned short> v_ushort{ 10, 20, 30 };
		REQUIRE(v_ushort.m_X == 10);
		REQUIRE(v_ushort.m_Y == 20);
		REQUIRE(v_ushort.m_Z == 30);
		Vector3<unsigned long> v_ulong{ 10, 20, 30 };
		REQUIRE(v_ulong.m_X == 10);
		REQUIRE(v_ulong.m_Y == 20);
		REQUIRE(v_ulong.m_Z == 30);
		Vector3<unsigned long long> v_ulong_long{ 10, 20, 30 };
		REQUIRE(v_ulong_long.m_X == 10);
		REQUIRE(v_ulong_long.m_Y == 20);
		REQUIRE(v_ulong_long.m_Z == 30);
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
		REQUIRE(v_int_cross.m_X == 0);
		REQUIRE(v_int_cross.m_Y == 0);
		REQUIRE(v_int_cross.m_Z == 0);
		REQUIRE(v_int.Normalize().Length() == 0);

		Vector3<float> v_float{ 10.0f, 20.0f, 30.0f };
		Vector3<float> v_float2{ 1.0f, 2.0f, 3.0f };
		REQUIRE(v_float.Length() == 37.416656494f);
		REQUIRE(v_float.LengthSquared() == 1400.0f);
		REQUIRE(v_float.Dot(v_float2) == 140.0f);
		Vector3<float> v_float_cross = v_float.Cross(v_float2);
		REQUIRE(v_float_cross.m_X == 0.0f);
		REQUIRE(v_float_cross.m_Y == 0.0f);
		REQUIRE(v_float_cross.m_Z == 0.0f);
		REQUIRE(v_float.Normalize().Length() == Catch::Approx(1.0f));
	}

	SECTION("Vector3 - operators")
	{
		Vector3<int> v_int{ 10, 20, 30 };
		Vector3<int> v_int2{ 1, 2, 3 };

		{
			const Vector3<int> v_int_add_res = v_int + v_int2;
			REQUIRE(v_int_add_res.m_X == 11);
			REQUIRE(v_int_add_res.m_Y == 22);
			REQUIRE(v_int_add_res.m_Z == 33);
			const Vector3<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res.m_X == 9);
			REQUIRE(v_int_sub_res.m_Y == 18);
			REQUIRE(v_int_sub_res.m_Z == 27);
			const Vector3<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res.m_X == 10);
			REQUIRE(v_int_mul_res.m_Y == 40);
			REQUIRE(v_int_mul_res.m_Z == 90);
			const Vector3<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res.m_X == 10);
			REQUIRE(v_int_div_res.m_Y == 10);
			REQUIRE(v_int_div_res.m_Z == 10);
		}

		{
			Vector3<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 22);
			REQUIRE(tmp.m_Z == 33);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 18);
			REQUIRE(tmp.m_Z == 27);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 40);
			REQUIRE(tmp.m_Z == 90);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 10);
			REQUIRE(tmp.m_Z == 10);
		}

		{
			Vector3<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 21);
			REQUIRE(tmp.m_Z == 31);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 19);
			REQUIRE(tmp.m_Z == 29);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp.m_X == 20);
			REQUIRE(tmp.m_Y == 40);
			REQUIRE(tmp.m_Z == 60);
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
			REQUIRE(v_float_cast.m_X == 10.0f);
			REQUIRE(v_float_cast.m_Y == 20.0f);
			REQUIRE(v_float_cast.m_Z == 30.0f);
			Vector3<double> v_double_cast = v_int.Cast<double>();
			REQUIRE(v_double_cast.m_X == 10.0);
			REQUIRE(v_double_cast.m_Y == 20.0);
			REQUIRE(v_double_cast.m_Z == 30.0);
			Vector3<long> v_long_cast = v_int.Cast<long>();
			REQUIRE(v_long_cast.m_X == 10);
			REQUIRE(v_long_cast.m_Y == 20);
			REQUIRE(v_long_cast.m_Z == 30);
			Vector3<long long> v_long_long_cast = v_int.Cast<long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			Vector3<short> v_short_cast = v_int.Cast<short>();
			REQUIRE(v_short_cast.m_X == 10);
			REQUIRE(v_short_cast.m_Y == 20);
			REQUIRE(v_short_cast.m_Z == 30);
			Vector3<char> v_char_cast = v_int.Cast<char>();
			REQUIRE(v_char_cast.m_X == 10);
			REQUIRE(v_char_cast.m_Y == 20);
			REQUIRE(v_char_cast.m_Z == 30);
			Vector3<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
			REQUIRE(v_uint_cast.m_X == 10);
			REQUIRE(v_uint_cast.m_Y == 20);
			REQUIRE(v_uint_cast.m_Z == 30);
			Vector3<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
			REQUIRE(v_uchar_cast.m_X == 10);
			REQUIRE(v_uchar_cast.m_Y == 20);
			REQUIRE(v_uchar_cast.m_Z == 30);
			Vector3<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
			REQUIRE(v_ushort_cast.m_X == 10);
			REQUIRE(v_ushort_cast.m_Y == 20);
			REQUIRE(v_ushort_cast.m_Z == 30);
			Vector3<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
			REQUIRE(v_ulong_cast.m_X == 10);
			REQUIRE(v_ulong_cast.m_Y == 20);
			REQUIRE(v_ulong_cast.m_Z == 30);
			Vector3<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
		}

		// Float to others
		{
			Vector3<int> v_int_cast = v_float.Cast<int>();
			REQUIRE(v_int_cast.m_X == 10);
			REQUIRE(v_int_cast.m_Y == 20);
			REQUIRE(v_int_cast.m_Z == 30);
			Vector3<double> v_double_cast = v_float.Cast<double>();
			REQUIRE(v_double_cast.m_X == 10.0);
			REQUIRE(v_double_cast.m_Y == 20.0);
			REQUIRE(v_double_cast.m_Z == 30.0);
			Vector3<long> v_long_cast = v_float.Cast<long>();
			REQUIRE(v_long_cast.m_X == 10);
			REQUIRE(v_long_cast.m_Y == 20);
			REQUIRE(v_long_cast.m_Z == 30);
			Vector3<long long> v_long_long_cast = v_float.Cast<long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			Vector3<short> v_short_cast = v_float.Cast<short>();
			REQUIRE(v_short_cast.m_X == 10);
			REQUIRE(v_short_cast.m_Y == 20);
			REQUIRE(v_short_cast.m_Z == 30);
			Vector3<char> v_char_cast = v_float.Cast<char>();
			REQUIRE(v_char_cast.m_X == 10);
			REQUIRE(v_char_cast.m_Y == 20);
			REQUIRE(v_char_cast.m_Z == 30);
			Vector3<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
			REQUIRE(v_uint_cast.m_X == 10);
			REQUIRE(v_uint_cast.m_Y == 20);
			REQUIRE(v_uint_cast.m_Z == 30);
			Vector3<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
			REQUIRE(v_uchar_cast.m_X == 10);
			REQUIRE(v_uchar_cast.m_Y == 20);
			REQUIRE(v_uchar_cast.m_Z == 30);
			Vector3<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
			REQUIRE(v_ushort_cast.m_X == 10);
			REQUIRE(v_ushort_cast.m_Y == 20);
			REQUIRE(v_ushort_cast.m_Z == 30);
			Vector3<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
			REQUIRE(v_ulong_cast.m_X == 10);
			REQUIRE(v_ulong_cast.m_Y == 20);
			REQUIRE(v_ulong_cast.m_Z == 30);
			Vector3<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
		}
	}
}

TEST_CASE("Vector4", "[Math]")
{
	using namespace engine::math;

	SECTION("Vector4 - constructors")
	{
		Vector4<int> v_int{ 10, 20, 30, 40 };
		REQUIRE(v_int.m_X == 10);
		REQUIRE(v_int.m_Y == 20);
		REQUIRE(v_int.m_Z == 30);
		REQUIRE(v_int.m_A == 40);
		Vector4<float> v_float{ 10.0f, 20.0f, 30.f, 40.f };
		REQUIRE(v_float.m_X == 10.0f);
		REQUIRE(v_float.m_Y == 20.0f);
		REQUIRE(v_float.m_Z == 30.0f);
		REQUIRE(v_float.m_A == 40.0f);
		Vector4<double> v_double{ 10.0, 20.0, 30.0, 40.0 };
		REQUIRE(v_double.m_X == 10.0);
		REQUIRE(v_double.m_Y == 20.0);
		REQUIRE(v_double.m_Z == 30.0);
		REQUIRE(v_double.m_A == 40.0);
		Vector4<long> v_long{ 10, 20, 30, 40 };
		REQUIRE(v_long.m_X == 10);
		REQUIRE(v_long.m_Y == 20);
		REQUIRE(v_long.m_Z == 30);
		REQUIRE(v_long.m_A == 40);
		Vector4<long long> v_long_long{ 10, 20, 30, 40 };
		REQUIRE(v_long_long.m_X == 10);
		REQUIRE(v_long_long.m_Y == 20);
		REQUIRE(v_long_long.m_Z == 30);
		REQUIRE(v_long_long.m_A == 40);
		Vector4<short> v_short{ 10, 20, 30, 40 };
		REQUIRE(v_short.m_X == 10);
		REQUIRE(v_short.m_Y == 20);
		REQUIRE(v_short.m_Z == 30);
		REQUIRE(v_short.m_A == 40);
		Vector4<char> v_char{ 10, 20, 30, 40 };
		REQUIRE(v_char.m_X == 10);
		REQUIRE(v_char.m_Y == 20);
		REQUIRE(v_char.m_Z == 30);
		REQUIRE(v_char.m_A == 40);
		Vector4<unsigned int> v_uint{ 10, 20, 30, 40 };
		REQUIRE(v_uint.m_X == 10);
		REQUIRE(v_uint.m_Y == 20);
		REQUIRE(v_uint.m_Z == 30);
		REQUIRE(v_uint.m_A == 40);
		Vector4<unsigned char> v_uchar{ 10, 20, 30, 40 };
		REQUIRE(v_uchar.m_X == 10);
		REQUIRE(v_uchar.m_Y == 20);
		REQUIRE(v_uchar.m_Z == 30);
		REQUIRE(v_uchar.m_A == 40);
		Vector4<unsigned short> v_ushort{ 10, 20, 30, 40 };
		REQUIRE(v_ushort.m_X == 10);
		REQUIRE(v_ushort.m_Y == 20);
		REQUIRE(v_ushort.m_Z == 30);
		REQUIRE(v_ushort.m_A == 40);
		Vector4<unsigned long> v_ulong{ 10, 20, 30, 40 };
		REQUIRE(v_ulong.m_X == 10);
		REQUIRE(v_ulong.m_Y == 20);
		REQUIRE(v_ulong.m_Z == 30);
		REQUIRE(v_ulong.m_A == 40);
		Vector4<unsigned long long> v_ulong_long{ 10, 20, 30, 40 };
		REQUIRE(v_ulong_long.m_X == 10);
		REQUIRE(v_ulong_long.m_Y == 20);
		REQUIRE(v_ulong_long.m_Z == 30);
		REQUIRE(v_ulong_long.m_A == 40);
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
		REQUIRE(v_int_cross.m_X == 0);
		REQUIRE(v_int_cross.m_Y == 0);
		REQUIRE(v_int_cross.m_Z == 0);
		REQUIRE(v_int_cross.m_A == 0);
		REQUIRE(v_int.Normalize().Length() == 0);

		Vector4<float> v_float{ 10.0f, 20.0f, 30.0f, 40.f };
		Vector4<float> v_float2{ 1.0f, 2.0f, 3.0f, 4.f };
		REQUIRE(v_float.Length() == 54.7817650f);
		REQUIRE(v_float.LengthSquared() == 3000.0f);
		REQUIRE(v_float.Dot(v_float2) == 300.0f);
		Vector4<float> v_float_cross = v_float.Cross(v_float2);
		REQUIRE(v_float_cross.m_X == 0.0f);
		REQUIRE(v_float_cross.m_Y == 0.0f);
		REQUIRE(v_float_cross.m_Z == 0.0f);
		REQUIRE(v_float_cross.m_A == 0.0f);
		REQUIRE(v_float.Normalize().Length() == Catch::Approx(1.0f).margin(1.e-2));
	}

	SECTION("Vector4 - operators")
	{
		Vector4<int> v_int{ 10, 20, 30 , 40};
		Vector4<int> v_int2{ 1, 2, 3, 4 };

		{
			const Vector4<int> v_int_add_res = v_int + v_int2;
			REQUIRE(v_int_add_res.m_X == 11);
			REQUIRE(v_int_add_res.m_Y == 22);
			REQUIRE(v_int_add_res.m_Z == 33);
			REQUIRE(v_int_add_res.m_A == 44);
			const Vector4<int> v_int_sub_res = v_int - v_int2;
			REQUIRE(v_int_sub_res.m_X == 9);
			REQUIRE(v_int_sub_res.m_Y == 18);
			REQUIRE(v_int_sub_res.m_Z == 27);
			REQUIRE(v_int_sub_res.m_A == 36);
			const Vector4<int> v_int_mul_res = v_int * v_int2;
			REQUIRE(v_int_mul_res.m_X == 10);
			REQUIRE(v_int_mul_res.m_Y == 40);
			REQUIRE(v_int_mul_res.m_Z == 90);
			REQUIRE(v_int_mul_res.m_A == 160);
			const Vector4<int> v_int_div_res = v_int / v_int2;
			REQUIRE(v_int_div_res.m_X == 10);
			REQUIRE(v_int_div_res.m_Y == 10);
			REQUIRE(v_int_div_res.m_Z == 10);
			REQUIRE(v_int_div_res.m_A == 10);
		}

		{
			Vector4<int> tmp = v_int;
			tmp += v_int2;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 22);
			REQUIRE(tmp.m_Z == 33);
			REQUIRE(tmp.m_A == 44);
			tmp = v_int;
			tmp -= v_int2;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 18);
			REQUIRE(tmp.m_Z == 27);
			REQUIRE(tmp.m_A == 36);
			tmp = v_int;
			tmp *= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 40);
			REQUIRE(tmp.m_Z == 90);
			REQUIRE(tmp.m_A == 160);
			tmp = v_int;
			tmp /= v_int2;
			REQUIRE(tmp.m_X == 10);
			REQUIRE(tmp.m_Y == 10);
			REQUIRE(tmp.m_Z == 10);
			REQUIRE(tmp.m_A == 10);
		}

		{
			Vector4<int> tmp = v_int;
			tmp += 1;
			REQUIRE(tmp.m_X == 11);
			REQUIRE(tmp.m_Y == 21);
			REQUIRE(tmp.m_Z == 31);
			REQUIRE(tmp.m_A == 41);
			tmp = v_int;
			tmp -= 1;
			REQUIRE(tmp.m_X == 9);
			REQUIRE(tmp.m_Y == 19);
			REQUIRE(tmp.m_Z == 29);
			REQUIRE(tmp.m_A == 39);
			tmp = v_int;
			tmp *= 2;
			REQUIRE(tmp.m_X == 20);
			REQUIRE(tmp.m_Y == 40);
			REQUIRE(tmp.m_Z == 60);
			REQUIRE(tmp.m_A == 80);
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
			REQUIRE(v_float_cast.m_X == 10.0f);
			REQUIRE(v_float_cast.m_Y == 20.0f);
			REQUIRE(v_float_cast.m_Z == 30.0f);
			REQUIRE(v_float_cast.m_A == 40.0f);
			Vector4<double> v_double_cast = v_int.Cast<double>();
			REQUIRE(v_double_cast.m_X == 10.0);
			REQUIRE(v_double_cast.m_Y == 20.0);
			REQUIRE(v_double_cast.m_Z == 30.0);
			REQUIRE(v_double_cast.m_A == 40.0);
			Vector4<long> v_long_cast = v_int.Cast<long>();
			REQUIRE(v_long_cast.m_X == 10);
			REQUIRE(v_long_cast.m_Y == 20);
			REQUIRE(v_long_cast.m_Z == 30);
			REQUIRE(v_long_cast.m_A == 40);
			Vector4<long long> v_long_long_cast = v_int.Cast<long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			REQUIRE(v_long_long_cast.m_A == 40);
			Vector4<short> v_short_cast = v_int.Cast<short>();
			REQUIRE(v_short_cast.m_X == 10);
			REQUIRE(v_short_cast.m_Y == 20);
			REQUIRE(v_short_cast.m_Z == 30);
			REQUIRE(v_short_cast.m_A == 40);
			Vector4<char> v_char_cast = v_int.Cast<char>();
			REQUIRE(v_char_cast.m_X == 10);
			REQUIRE(v_char_cast.m_Y == 20);
			REQUIRE(v_char_cast.m_Z == 30);
			REQUIRE(v_char_cast.m_A == 40);
			Vector4<unsigned int> v_uint_cast = v_int.Cast<unsigned int>();
			REQUIRE(v_uint_cast.m_X == 10);
			REQUIRE(v_uint_cast.m_Y == 20);
			REQUIRE(v_uint_cast.m_Z == 30);
			REQUIRE(v_uint_cast.m_A == 40);
			Vector4<unsigned char> v_uchar_cast = v_int.Cast<unsigned char>();
			REQUIRE(v_uchar_cast.m_X == 10);
			REQUIRE(v_uchar_cast.m_Y == 20);
			REQUIRE(v_uchar_cast.m_Z == 30);
			REQUIRE(v_uchar_cast.m_A == 40);
			Vector4<unsigned short> v_ushort_cast = v_int.Cast<unsigned short>();
			REQUIRE(v_ushort_cast.m_X == 10);
			REQUIRE(v_ushort_cast.m_Y == 20);
			REQUIRE(v_ushort_cast.m_Z == 30);
			REQUIRE(v_ushort_cast.m_A == 40);
			Vector4<unsigned long> v_ulong_cast = v_int.Cast<unsigned long>();
			REQUIRE(v_ulong_cast.m_X == 10);
			REQUIRE(v_ulong_cast.m_Y == 20);
			REQUIRE(v_ulong_cast.m_Z == 30);
			REQUIRE(v_ulong_cast.m_A == 40);
			Vector4<unsigned long long> v_ulong_long_cast = v_int.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			REQUIRE(v_long_long_cast.m_A == 40);
		}

		// Float to others
		{
			Vector4<int> v_int_cast = v_float.Cast<int>();
			REQUIRE(v_int_cast.m_X == 10);
			REQUIRE(v_int_cast.m_Y == 20);
			REQUIRE(v_int_cast.m_Z == 30);
			REQUIRE(v_int_cast.m_A == 40);
			Vector4<double> v_double_cast = v_float.Cast<double>();
			REQUIRE(v_double_cast.m_X == 10.0);
			REQUIRE(v_double_cast.m_Y == 20.0);
			REQUIRE(v_double_cast.m_Z == 30.0);
			REQUIRE(v_double_cast.m_A == 40.0);
			Vector4<long> v_long_cast = v_float.Cast<long>();
			REQUIRE(v_long_cast.m_X == 10);
			REQUIRE(v_long_cast.m_Y == 20);
			REQUIRE(v_long_cast.m_Z == 30);
			REQUIRE(v_long_cast.m_A == 40);
			Vector4<long long> v_long_long_cast = v_float.Cast<long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			REQUIRE(v_long_long_cast.m_A == 40);
			Vector4<short> v_short_cast = v_float.Cast<short>();
			REQUIRE(v_short_cast.m_X == 10);
			REQUIRE(v_short_cast.m_Y == 20);
			REQUIRE(v_short_cast.m_Z == 30);
			REQUIRE(v_short_cast.m_A == 40);
			Vector4<char> v_char_cast = v_float.Cast<char>();
			REQUIRE(v_char_cast.m_X == 10);
			REQUIRE(v_char_cast.m_Y == 20);
			REQUIRE(v_char_cast.m_Z == 30);
			REQUIRE(v_char_cast.m_A == 40);
			Vector4<unsigned int> v_uint_cast = v_float.Cast<unsigned int>();
			REQUIRE(v_uint_cast.m_X == 10);
			REQUIRE(v_uint_cast.m_Y == 20);
			REQUIRE(v_uint_cast.m_Z == 30);
			REQUIRE(v_uint_cast.m_A == 40);
			Vector4<unsigned char> v_uchar_cast = v_float.Cast<unsigned char>();
			REQUIRE(v_uchar_cast.m_X == 10);
			REQUIRE(v_uchar_cast.m_Y == 20);
			REQUIRE(v_uchar_cast.m_Z == 30);
			REQUIRE(v_uchar_cast.m_A == 40);
			Vector4<unsigned short> v_ushort_cast = v_float.Cast<unsigned short>();
			REQUIRE(v_ushort_cast.m_X == 10);
			REQUIRE(v_ushort_cast.m_Y == 20);
			REQUIRE(v_ushort_cast.m_Z == 30);
			REQUIRE(v_ushort_cast.m_A == 40);
			Vector4<unsigned long> v_ulong_cast = v_float.Cast<unsigned long>();
			REQUIRE(v_ulong_cast.m_X == 10);
			REQUIRE(v_ulong_cast.m_Y == 20);
			REQUIRE(v_ulong_cast.m_Z == 30);
			REQUIRE(v_ulong_cast.m_A == 40);
			Vector4<unsigned long long> v_ulong_long_cast = v_float.Cast<unsigned long long>();
			REQUIRE(v_long_long_cast.m_X == 10);
			REQUIRE(v_long_long_cast.m_Y == 20);
			REQUIRE(v_long_long_cast.m_Z == 30);
			REQUIRE(v_long_long_cast.m_A == 40);
		}
	}
}