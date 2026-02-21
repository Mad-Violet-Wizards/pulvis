#include "Quadtree.hpp"
#include "QuadtreeTestsMoc.hpp"

#include <catch2/catch2.hpp>
#include <random>

using namespace pulvis::tl;
using namespace pulvis::tl::tests;

static Quadtree<STestEntity, 4, 4> MakeTree4x4(const Rect<float>& _bounds)
{
	FastFunction get_bounds_fn = FastFunction::Make<Rect<float>, const STestEntity&>([](const STestEntity& entity) {
		return entity.Bounds;
		});
	return Quadtree<STestEntity, 4, 4>(_bounds, std::move(get_bounds_fn));
}

static Quadtree<STestEntity, 8, 8> MakeTree8x8(const Rect<float>& _bounds)
{
	FastFunction get_bounds_fn = FastFunction::Make<Rect<float>, const STestEntity&>([](const STestEntity& entity) {
		return entity.Bounds;
		});
	return Quadtree<STestEntity, 8, 8>(_bounds, std::move(get_bounds_fn));
}

TEST_CASE("Quadtree", "[TL][Quadtree]")
{
	SECTION("Insert and count?")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		CHECK(tree.Count() == 0);

		tree.Insert({ 1, {10.f, 10.f, 20.f, 20.f } });
		CHECK(tree.Count() == 1);

		tree.Insert({ 2, {500.f, 500.f, 20.f, 20.f} });
		CHECK(tree.Count() == 2);

		tree.Insert({ 3, {900.f, 900.f, 20.f, 20.f} });
		CHECK(tree.Count() == 3);
	}

	SECTION("Insert outside bounds is ignored")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 100.f, 100.f });

		tree.Insert({ 1, { 200.f, 200.f, 10.f, 10.f } });
		CHECK(tree.Count() == 0);

		tree.Insert({ 2, { -50.f, -50.f, 10.f, 10.f } });
		CHECK(tree.Count() == 0);
	}

	SECTION("Query returns elements in area")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		tree.Insert({ 1, { 10.f, 10.f, 20.f, 20.f } });
		tree.Insert({ 2, {500.f, 500.f, 20.f, 20.f} });
		tree.Insert({ 3, { 900.f, 900.f, 20.f, 20.f} });

		std::vector<STestEntity> results;

		tree.Query({ 0.f, 0.f, 100.f, 100.f }, results);
		CHECK(results.size() == 1);
		CHECK(results[0].ID == 1);

		results.clear();
		tree.Query({ 400.f, 400.f, 200.f, 200.f }, results);
		CHECK(results.size() == 1);
		CHECK(results[0].ID == 2);

		results.clear();
		tree.Query({ 800.f, 800.f, 200.f, 200.f }, results);
		CHECK(results.size() == 1);
		CHECK(results[0].ID == 3);

		results.clear();
		tree.Query({ 0.f, 0.f, 1000.f, 1000.f }, results);
		CHECK(results.size() == 3);
	}

	SECTION("Query entire area returns all elements")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		for (unsigned int i = 0; i < 20; ++i)
		{
			float pos = static_cast<float>(i * 45);
			tree.Insert({ i, { pos, pos, 10.f, 10.f } });
		}

		std::vector<STestEntity> results;
		tree.Query({ 0.f, 0.f, 1000.f, 1000.f }, results);
		CHECK(results.size() == 20);
	}

	SECTION("Query empty area returns nothing")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		tree.Insert({ 1, { 10.f, 10.f, 20.f, 20.f } });

		std::vector<STestEntity> results;
		tree.Query({ 800.f, 800.f, 50.f, 50.f }, results);
		CHECK(results.empty());
	}

	SECTION("Subdivision occurs when exceeding MAX_ELEMENTS")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		tree.Insert({ 0, { 100.f, 100.f, 5.f, 5.f } });
		tree.Insert({ 1, { 600.f, 100.f, 5.f, 5.f } });
		tree.Insert({ 2, { 100.f, 600.f, 5.f, 5.f } });
		tree.Insert({ 3, { 600.f, 600.f, 5.f, 5.f } });

		CHECK(tree.Count() == 4);
		CHECK(tree.NodeCount() == 1);

		tree.Insert({ 4, { 200.f, 200.f, 5.f, 5.f } });

		CHECK(tree.Count() == 5);
		CHECK(tree.NodeCount() == 5);

		std::vector<STestEntity> results;
		tree.Query({ 0.f, 0.f, 1000.f, 1000.f }, results);
		CHECK(results.size() == 5);
	}

	SECTION("Elements on boundaries stay in parent")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 100.f, 100.f });

		// Force subdivision
		for (unsigned int i = 0; i < 5; ++i)
		{
			tree.Insert({ i, { 5.f + static_cast<float>(i), 5.f, 2.f, 2.f } });
		}

		tree.Insert({ 99, { 45.f, 45.f, 20.f, 20.f } });
		CHECK(tree.Count() == 6);

		std::vector<STestEntity> results;
		tree.Query({ 40.f, 40.f, 30.f, 30.f }, results);

		bool found = false;
		for (const STestEntity& entity : results)
		{
			if (entity.ID == 99)
			{
				found = true;
				break;
			}
		}

		CHECK(found);
	}

	SECTION("Clear resets the tree")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });


		for (unsigned int i = 0; i < 999; ++i)
		{
			tree.Insert({ i, { static_cast<float>(i % 100), static_cast<float>(i / 100), 1.f, 1.f } });
		}

		CHECK(tree.Count() == 999);

		tree.Clear();
		CHECK(tree.Count() == 0);
		CHECK(tree.NodeCount() == 1);

		std::vector<STestEntity> results;
		tree.Query({ 0.f, 0.f, 100.f, 100.f }, results);
		CHECK(results.empty());
	}

	SECTION("Clear and reinsert works correctly")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

		tree.Insert({ 1, { 10.f, 10.f, 20.f, 20.f } });
		tree.Clear();
		tree.Insert({ 2, { 500.f, 500.f, 20.f, 20.f } });

		CHECK(tree.Count() == 1);

		{
			std::vector<STestEntity> results;
			tree.Query({ 0.f, 0.f, 100.f, 100.f }, results);
			CHECK(results.empty());
		}

		{
			std::vector<STestEntity> results;
			tree.Query({ 500.f, 500.f, 20.f, 20.f }, results);
			CHECK(results.size() == 1);
			CHECK(results[0].ID == 2);
		}

		SECTION("Rebuild repopulates the tree")
		{
			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

			tree.Insert({ 1, { 10.f, 10.f, 20.f, 20.f } });
			tree.Insert({ 2, { 500.f, 500.f, 20.f, 20.f } });

			std::vector<STestEntity> new_elements = {
				{ 3, { 100.f, 100.f, 20.f, 20.f } },
				{ 4, { 600.f, 600.f, 20.f, 20.f } },
				{ 5, { 900.f, 900.f, 20.f, 20.f }  }
			};

			tree.Rebuild(new_elements);
			CHECK(tree.Count() == 3);

			std::vector<STestEntity> results;
			tree.Query({ 200.f, 200.f, 800, 800.f }, results);
			CHECK(results.size() == 2);
		}

		SECTION("Many elements in same position")
		{
			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 1000.f, 1000.f });

			for (unsigned int i = 0; i < 20; ++i)
			{
				tree.Insert({ i, { 100.f, 100.f, 10.f, 10.f } });
			}

			CHECK(tree.Count() == 20);

			std::vector<STestEntity> results;
			tree.Query({ 95.f, 95.f, 20.f, 20.f }, results);
			CHECK(results.size() == 20);
		}

		SECTION("Lambda as bounds function")
		{
			Quadtree<STestEntity, 4, 4> tree(
				{ 0.f, 0.f, 1000.f, 1000.f },
				FastFunction::Make<Rect<float>, const STestEntity&>(
					[](const STestEntity& _e) -> Rect<float> { return _e.Bounds; }
				)
			);

			tree.Insert({ 1, { 10.f, 10.f, 20.f, 20.f } });
			CHECK(tree.Count() == 1);
		}
	}
}

#define RUN_BENCHMARK
#ifdef RUN_BENCHMARK
TEST_CASE("Quadtree const data performance", "[TL][Quadtree][Benchmark]")
{
	SECTION("Performance tests")
	{
		BENCHMARK("Insert 1000 elements (1000 elements, const data, depth = 4 max_elements = 4)")
		{
			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 95.f;
				const float y = static_cast<float>(i / 100) * 95.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}
			return tree.Count();
		};

		BENCHMARK("Insert 1000 elements (1000 elements, const data, depth = 8, max_elements = 8)")
		{
			Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 100000.f, 100000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 950.f;
				const float y = static_cast<float>(i / 100) * 950.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}
			return tree.Count();
		};

		BENCHMARK("Query small area (1000 elements, const data, depth = 4, max_elements = 4)")
		{
			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 95.f;
				const float y = static_cast<float>(i / 100) * 95.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}

			std::vector<STestEntity> results;
			results.reserve(32);
			tree.Query({ 100.f, 100.f, 200.f, 200.f }, results);
			return results.size();
		};

		BENCHMARK("Query large area (1000 elements, const data, depth = 4, max_elements = 4)")
		{
			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 95.f;
				const float y = static_cast<float>(i / 100) * 95.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}

			std::vector<STestEntity> results;
			results.reserve(512);
			tree.Query({ 0.f, 0.f, 5000.f, 5000.f }, results);
			return results.size();
		};

		BENCHMARK("Query small area (1000 elements, const data, depth = 8, max_elements = 8)")
		{
			Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 10000.f, 10000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 95.f;
				const float y = static_cast<float>(i / 100) * 95.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}

			std::vector<STestEntity> results;
			results.reserve(32);
			tree.Query({ 100.f, 100.f, 200.f, 200.f }, results);
			return results.size();
		};

		BENCHMARK("Query large area (1000 elements, const data, depth = 8, max_elements = 8)")
		{
			Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 10000.f, 10000.f });
			for (unsigned int i = 0; i < 1000; ++i)
			{
				const float x = static_cast<float>(i % 100) * 95.f;
				const float y = static_cast<float>(i / 100) * 95.f;
				tree.Insert({ i, { x, y, 10.f, 10.f } });
			}

			std::vector<STestEntity> results;
			results.reserve(512);
			tree.Query({ 0.f, 0.f, 5000.f, 5000.f }, results);
			return results.size();
		};

		BENCHMARK("Clear and Rebuild (1000 element, depth = 4, max_elements = 4)")
		{
			const std::vector<STestEntity> elements = []()
				{
					std::vector<STestEntity> generator;
					for (unsigned int i = 0; i < 1000; ++i)
					{
						float x = static_cast<float>(i % 100) * 95.f;
						float y = static_cast<float>(i / 100) * 95.f;
						generator.push_back({ i, { x, y, 10.f, 10.f } });
					}
					return generator;
				}();

			Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
			tree.Rebuild(elements);
			return tree.Count();
		};

		BENCHMARK("Clear and Rebuild (1000 element, depth = 8, max_elements = 8)")
		{
			const std::vector<STestEntity> elements = []()
				{
					std::vector<STestEntity> generator;
					for (unsigned int i = 0; i < 1000; ++i)
					{
						float x = static_cast<float>(i % 100) * 95.f;
						float y = static_cast<float>(i / 100) * 95.f;
						generator.push_back({ i, { x, y, 10.f, 10.f } });
					}
					return generator;
				}();

			Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 10000.f, 10000.f });
			tree.Rebuild(elements);
			return tree.Count();
		};
	}
}

TEST_CASE("Quadtree random data performance", "[TL][Quadtree][Benchmark]")
{
	static std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist_pos(0.f, 10000.f);
	std::uniform_real_distribution<float> dist_size(0.f, 250.f);

	BENCHMARK("Insert 1000 elements (random data, depth = 4 max_elements = 4)")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}
		return tree.Count();
	};

	BENCHMARK("Insert 1000 elements (random data, depth = 8, max_elements = 8)")
	{
		Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 100000.f, 100000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}
		return tree.Count();
	};

	BENCHMARK("Query small area (1000 elements, random data, depth = 4, max_elements = 4)")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}

		std::vector<STestEntity> results;
		tree.Query({ 100.f, 100.f, 200.f, 200.f }, results);
		return results.size();
	};

	BENCHMARK("Query large area (1000 elements, random data, depth = 4, max_elements = 4)")
	{
		Quadtree<STestEntity, 4, 4> tree = MakeTree4x4({ 0.f, 0.f, 10000.f, 10000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}

		std::vector<STestEntity> results;
		results.reserve(512);
		tree.Query({ 0.f, 0.f, 5000.f, 5000.f }, results);
		return results.size();
	};

	BENCHMARK("Query small area (1000 elements, random data, depth = 8, max_elements = 8)")
	{
		Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 10000.f, 10000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}

		std::vector<STestEntity> results;
		results.reserve(32);
		tree.Query({ 100.f, 100.f, 200.f, 200.f }, results);
		return results.size();
	};

	BENCHMARK("Query large area (1000 elements, random data, depth = 8, max_elements = 8)")
	{
		Quadtree<STestEntity, 8, 8> tree = MakeTree8x8({ 0.f, 0.f, 10000.f, 10000.f });
		for (unsigned int i = 0; i < 1000; ++i)
		{
			const float x = dist_pos(rng);
			const float y = dist_pos(rng);

			const float width = dist_size(rng);
			const float height = dist_size(rng);

			tree.Insert({ i, { x, y, width, height } });
		}

		std::vector<STestEntity> results;
		results.reserve(512);
		tree.Query({ 0.f, 0.f, 5000.f, 5000.f }, results);
		return results.size();
	};
}
#endif