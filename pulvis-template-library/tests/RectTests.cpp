#include "Rect.hpp"

#include <catch2/catch2.hpp>
#include <iostream>
#include <optional>

using namespace pulvis::tl;


TEST_CASE("Rect", "[TL][Rect]")
{
	SECTION("Constructors")
	{
		Rect<int> rect1(0, 0, 800, 600);
		CHECK(rect1.Position == glm::vec<2, int>(0, 0));
		CHECK(rect1.Size == glm::vec<2, int>(800, 600));

		Rect<int> rect2;
		CHECK(rect2.Position == glm::vec<2, int>(0, 0));
		CHECK(rect2.Size == glm::vec<2, int>(0, 0));

		Rect<int> rect3({ 10, 20 }, { 30, 40 });
		CHECK(rect3.Position == glm::vec<2, int>(10, 20));
		CHECK(rect3.Size == glm::vec<2, int>(30, 40));
	}

	SECTION("Contains")
	{
		Rect<int> rect(0, 0, 800, 600);
		CHECK(rect.ContainsPoint({ 10, 10 }));
		CHECK(rect.ContainsPoint({ 0, 0 }));
		CHECK(rect.ContainsPoint({ 800, 600 }));
		CHECK(!rect.ContainsPoint({ -1, -1 }));
		CHECK(!rect.ContainsPoint({ 801, 601 }));
	}

	SECTION("Intersects")
	{
		Rect<int> rect1(0, 0, 800, 600);
		Rect<int> rect2(10, 10, 30, 40);

		std::optional<glm::vec<2, int>> intersection = rect1.IntersectPoint(rect2);
		CHECK(intersection.has_value());
		CHECK(intersection.value() == glm::vec<2, int>(10, 10));
		CHECK(rect1.Intersect(rect2) == true);

		Rect<int> rect3(801, 601, 10, 10);
		auto no_intersection = rect1.IntersectPoint(rect3);
		CHECK(!no_intersection.has_value());
		CHECK(rect1.Intersect(rect3) == false);
	}

	SECTION("Center")
	{
		Rect<int> rect(0, 0, 800, 600);
		CHECK(rect.GetCenter() == glm::vec<2, int>(400, 300));
	}
}

