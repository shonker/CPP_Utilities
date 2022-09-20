﻿#include <iostream>
#include "include/utils/console/colour.h"

#include "include/utils/containers/memberwise_operators_array.h"
#include "include/utils/containers/memberwise_operators/seq.h"
#include "include/utils/containers/memberwise_operators/par.h"
#include "include/utils/math/vec.h"
#include "include/utils/output/std_containers.h"
#include "include/utils/math/angle.h"
#include "include/utils/graphics/colour.h"

#include "include/utils/containers/matrix.h"
#include "include/utils/logger.h"

#include "include/utils/containers/handled_container.h"
int main()
	{
	using namespace utils::output;
	std::cout 
		<< utils::console::colour::foreground::blue       
		<< utils::console::colour::background::dark_white 
		<< "Hello" 
		<< utils::console::colour::background::white
		<< " "
		<< utils::console::colour::foreground::white
		<< utils::console::colour::background::yellow
		<< "world"
		<< utils::console::colour::foreground::white
		<< utils::console::colour::background::green
		<< "!"
		<< utils::console::color::restore_defaults;

	utils::math::vec<float, 4> a{5.f, 5.f, 5.f, 5.f};
	utils::math::vec<float, 3> b{0.f, 1.f, 2.f};

	a + 3.f;
	b + a;
	a + b;

	using namespace utils::containers::memberwise_operators::seq;
	std::vector<float> stdarr_a;
	std::vector<float> stdarr_out{1.f - stdarr_a + -stdarr_a};


	using namespace utils::output;
	std::cout << a << "\n";

	using namespace utils::math::angle::literals;
	auto deg{90_deg};
	auto rad{90_rad};
	auto sensicalrad{90_radpi};

	utils::math::angle::base<float, 1.f> other{.5};
	std::cout << deg << ", " << rad << ", " << sensicalrad << ", " << other << "\n";

	utils::graphics::colour::rgb_u colour_a{utils::graphics::colour::base::blue};
	utils::graphics::colour::rgb_u colour_b{utils::graphics::colour::base::cyan   , 128, 100};
	utils::graphics::colour::rgb_u colour_c{utils::graphics::colour::base::yellow , 200};
	utils::graphics::colour::rgb_u colour_d{utils::graphics::colour::base::magenta, 127};

	std::cout
		<< colour_a << "\n"
		<< colour_b << "\n"
		<< colour_c << "\n"
		<< colour_d << "\n";

	utils::containers::mat2x2f mat2x2f{1};
	utils::containers::mat3x3f mat3x3f;
	utils::containers::mat4x4f mat4x4f;
	mat2x2f += mat2x2f;
	std::cout << mat2x2f << "\n" << mat4x4f << "\n";


	utils::globals::logger.err("hi");
	utils::globals::logger.wrn("how");
	utils::globals::logger.log("are");
	utils::globals::logger.dgn("you");
	utils::globals::logger.inf("?");

	utils::containers::handled_container<int> handled;

	auto handle_0{handled.emplace(0)};
	auto handle_1{handled.emplace(1)};
	auto handle_2{handled.emplace(2)};

	handled.remove(handle_1);

	auto handle_3{handled.emplace(3)};

	}