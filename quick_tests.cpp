﻿#include <string>
#include <iostream>

#include "include/utils/memory.h"
#include "include/utils/console/colour.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/math/angle.h"
#include "include/utils/math/rect.h"

#include "include/utils/output/std_containers.h"
#include "include/utils/graphics/colour.h"

#include "include/utils/containers/matrix.h"
#include "include/utils/logger.h"

#include "include/utils/containers/handled_container.h"
#include "include/utils/containers/multihandled.h"

#include "include/utils/containers/hive/next.h"
#include "include/utils/containers/linked_vector.h"
#include "include/utils/containers/segmented_vector.h"

#include "include/utils/containers/multithreading/multiqueue_consumer.h"

#include "include/utils/containers/object_pool.h"

#include "include/utils/oop/counting.h"

#include "include/utils/math/geometry/shapes.h"
#include "include/utils/math/geometry/interactions.h"

#include "include/utils/math/geometry/voronoi/voronoi.h"

//using civ = utils::oop::counting_invalidating_move;
//
//struct test_struct : civ
//	{
//	test_struct(int v) : v{v} {}
//	int v;
//	};
int main()
	{
	utils::console::initializer console_initializer;

	using namespace utils::output;

	utils::math::geometry::circle circ{.center{0.f, 0.f}, .radius{5.f}};

	utils::math::geometry::point p{8.f, 1.f};
	
	static_assert(utils::math::geometry::concepts::shape<decltype(p)>);

	auto ret{circ.contains(p)};
	std::cout << ret << std::endl;

	utils::math::transform2 transform {{0.f, 0.f}, utils::math::angle::degf{90.f}, 2.f};
	utils::math::transform2 transform2{transform.inverse()};

	auto circ2{circ.transform(transform)};
	circ.transform_self(transform.inverse());

	std::cout << circ .contains(p) << std::endl;
	std::cout << circ2.contains(p) << std::endl;

	std::vector<utils::math::vec2f> points{{1.f, 2.f},{5.f, 6.f},{3.f, 2.f},{7.f, 8.f},{8.f, 3.f}};
	utils::math::geometry::aabb aabb{.ll{0.f}, .up{0.f}, .rr{10.f}, .dw{10.f}};
	utils::math::geometry::voronoi::graph voronoi{std::views::all(points), aabb};
	//static_assert(utils::math::geometry::concepts::shape<utils::math::geometry::convex_polygon>);
	//p.closest_point_and_distance(circ);
	//std::cout << circ.contains(circ);

	//if (auto opt{circ.collision(p)})
	//	{
	//	std::cout << opt.value();
	//	}

	aabb.contains(p);

	std::cout << utils::console::colour::restore_defaults << std::endl;
	for (size_t i = 0; i < 256; i++)
		{
		std::cout << utils::console::colour::foreground{utils::graphics::colour::rgb_u{0, i, 255 - i}};
		std::cout << utils::console::colour::background{utils::graphics::colour::rgb_u{0, 255 - i, i}};
		std::cout << "#";
		}
	std::cout << utils::console::colour::restore_defaults << std::endl;
	}