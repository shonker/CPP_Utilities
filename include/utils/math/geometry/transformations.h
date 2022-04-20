#pragma once

#include "../../concepts.h"

#include "../vec2.h"
#include "../angle.h"
#include "../transform2.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"
#include "aabb.h"
#include "point.h"

namespace utils::math::geometry::transformations
	{
#pragma region point
	// NOT to do //note, there's a space between to and do to not make it appear when searching for "TO[blank]DO"
#pragma endregion point

#pragma region segment
	inline geometry::segment operator*(const geometry::segment& segment, float scale) noexcept
		{
		return {segment.a * scale, segment.b * scale};
		}
	inline geometry::segment operator+(const geometry::segment& segment, math::angle::deg rotation) noexcept
		{
		return {segment.a + rotation, segment.b + rotation};
		}
	inline geometry::segment operator+(const geometry::segment& segment, vec2f translation) noexcept
		{
		return {segment.a + translation, segment.b + translation};
		}
	inline geometry::segment& operator*=(geometry::segment& segment, float scale) noexcept
		{
		segment = segment * scale;
		return segment;
		}
	inline geometry::segment& operator+=(geometry::segment& segment, math::angle::deg rotation) noexcept
		{
		segment = segment + rotation;
		return segment;
		}
	inline geometry::segment& operator+=(geometry::segment& segment, vec2f translation) noexcept
		{
		segment = segment + translation;
		return segment;
		}
#pragma endregion segment



#pragma region polygon
	inline geometry::polygon operator*(const geometry::polygon& polygon, float scale) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v *= scale; }
		return {std::move(new_vertices)};
		}
	
	inline geometry::polygon operator+(const geometry::polygon& polygon, math::angle::deg rotation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += rotation; }
		return {std::move(new_vertices)};
		}
	
	inline geometry::polygon operator+(const geometry::polygon& polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += translation; }
		return {std::move(new_vertices)};
		}
	
	inline geometry::polygon& operator*=(geometry::polygon& polygon, float scale) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v *= scale; }
		return polygon;
		}
	
	inline geometry::polygon& operator+=(geometry::polygon& polygon, math::angle::deg rotation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += rotation; }
		return polygon;
		}
	
	inline geometry::polygon& operator+=(geometry::polygon& polygon, vec2f translation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += translation; }
		return polygon;
		}

#pragma endregion polygon

#pragma region convex_polygon
	inline geometry::convex_polygon operator*(const geometry::convex_polygon& convex_polygon, float scale) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v *= scale; }
		return 	{ std::move(new_vertices) };
		}

	inline geometry::convex_polygon operator+(const geometry::convex_polygon& convex_polygon, math::angle::deg rotation) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v += rotation; }
		return 	{ std::move(new_vertices) };
		}

	inline geometry::convex_polygon operator+(const geometry::convex_polygon& convex_polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v += translation; }
		return 	{ std::move(new_vertices) };
		}

	inline geometry::convex_polygon& operator*=(geometry::convex_polygon& convex_polygon, float scale) noexcept
		{
		for (auto& v : convex_polygon.get_vertices()) 	{ v *= scale; }
		return convex_polygon;
		}

	inline geometry::convex_polygon& operator+=(geometry::convex_polygon& convex_polygon, math::angle::deg rotation) noexcept
		{
		for (auto& v : convex_polygon.get_vertices()) 	{ v += rotation; }
		return convex_polygon;
		}

	inline geometry::convex_polygon& operator+=(geometry::convex_polygon& convex_polygon, vec2f translation) noexcept
		{
		for (auto& v : convex_polygon.get_vertices()) 	{ v += translation; }
		return convex_polygon;
		}

#pragma endregion convex_polygon

#pragma region aabb
	inline geometry::aabb operator*(const geometry::aabb& aabb, float scale) noexcept
		{
		return 	{aabb.ll * scale, aabb.up * scale, aabb.rr * scale, aabb.dw * scale, };
		}

	inline geometry::aabb operator+(const geometry::aabb& aabb, vec2f translation) noexcept
		{
		return 	{aabb.ll + translation.x, aabb.up + translation.y, aabb.rr + translation.x, aabb.dw + translation.y};
		}

	/*namespace _
		{
		class proxy_rotation
			{
			const geometry::aabb& aabb;
			angle::deg        rotation;

		public:
			proxy_rotation(const geometry::aabb& aabb, math::angle::deg rotation) noexcept : aabb	{ aabb }, rotation	{ rotation }	{};

			operator geometry::aabb() 	{ return aabb; };
			//operator geometry::convex_polygon() 	{ return geometry::convex_polygon	{ aabb.ul, aabb.ur, aabb.dr, aabb.dl } + rotation; };
			};

		class proxy_transform
			{
			const geometry::aabb&  aabb;
			const transform2& transform;

		public:
			proxy_transform(const geometry::aabb& shape, const transform2& transform) noexcept : aabb	{ aabb }, transform	{ transform }	{};

			operator geometry::aabb()           	{ return static_cast<geometry::aabb>          ((aabb * transform.scaling) + transform.rotation) + transform.translation; };
			//operator geometry::convex_polygon() 	{ return static_cast<geometry::convex_polygon>((aabb * transform.scaling) + transform.rotation) + transform.translation; };
			};
		}
	
	inline auto operator+(const geometry::aabb& aabb, math::angle::deg rotation) noexcept
		{
		return _::proxy_rotation	{ aabb, rotation };
		}*/

	inline geometry::aabb operator+(const geometry::aabb& aabb, math::angle::deg rotation) noexcept
		{
		return aabb;
		}

	inline geometry::aabb& operator*=(geometry::aabb& aabb, float scale) noexcept
		{
		aabb = aabb * scale;
		return aabb;
		}
	inline geometry::aabb& operator+=(geometry::aabb& aabb, math::angle::deg rotation) noexcept
		{
		aabb = aabb + rotation;
		return aabb;
		}
	inline geometry::aabb& operator+=(geometry::aabb& aabb, vec2f translation) noexcept
		{
		aabb = aabb + translation;
		return aabb;
		}

	/*inline auto operator* (const geometry::aabb& shape, const transform2& transform) noexcept
		{
		return _::proxy_transform	{ shape, transform };
		}*/
#pragma endregion aabb

#pragma region circle
	inline geometry::circle operator*(const geometry::circle& circle, float scale) noexcept
		{
		return {circle.center * scale, circle.radius * scale};
		}
	inline geometry::circle operator+(const geometry::circle& circle, math::angle::deg rotation) noexcept
		{
		return {circle.center + rotation, circle.radius};
		}
	inline geometry::circle operator+(const geometry::circle& circle, vec2f translation) noexcept
		{
		return {circle.center + translation, circle.radius};
		}
	inline geometry::circle& operator*=(geometry::circle& circle, float scale) noexcept
		{
		circle = circle * scale;
		return circle;
		}
	inline geometry::circle& operator+=(geometry::circle& circle, math::angle::deg rotation) noexcept
		{
		circle = circle + rotation;
		return circle;
		}
	inline geometry::circle& operator+=(geometry::circle& circle, vec2f translation) noexcept
		{
		circle = circle + translation;
		return circle;
		}
#pragma endregion circle

	template <typename T>
	concept is_shape = ::utils::concepts::any_of<T, geometry::point, geometry::segment, geometry::aabb, geometry::circle, geometry::polygon, geometry::convex_polygon>;

	template <is_shape T>
	inline T  operator* (const T& shape, const transform2& transform) noexcept { return ((shape * transform.scaling) + transform.rotation) + transform.translation; }
	template <is_shape T>
	inline T& operator*=(T& shape, const transform2& transform) noexcept { shape = ((shape * transform.scaling) + transform.rotation) + transform.translation; return shape; }
	}