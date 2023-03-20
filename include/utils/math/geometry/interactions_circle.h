#pragma once


#include "inner_shapes.h"

namespace utils::math::geometry
	{
	
	vec2f circle::closest_point_to(const point& other) const noexcept
		{
		auto vec{other - center};
		vec.length = radius;
		return vec;
		}

	bool circle::contains(const point& other) const noexcept
		{
		return point{center}.distance_min(other) <= radius;
		}

	vec2f circle::closest_point_to(const segment& b) const noexcept
		{
		point closest_to_center{b.closest_point_to(point{center})};
		float dist_to_center{closest_to_center.distance_min(point{center})};
		if (dist_to_center >= radius)
			{
			return closest_point_to(point{closest_to_center});
			}
		else
			{
			return {}; //TODO intersections
			}
		}

	bool circle::intersects(const segment& other) const noexcept
		{
		if ( contains(other.a) && !contains(other.b)) { return true; }
		if ( contains(other.b) && !contains(other.a)) { return true; }
		if (!contains(other.b) && !contains(other.a)) { return other.distance_min(center) <= radius; }
		return false;
		}

	std::optional<vec2f> circle::intersection(const segment& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}

	bool circle::contains(const segment& other) const noexcept
		{
		return contains(other.a) && contains(other.b);
		}

	vec2f                circle::closest_point_to(const aabb& other) const noexcept { return {}; } //TODO
	vec2f                circle::distance_min    (const aabb& other) const noexcept { return {}; } //TODO
	bool                 circle::intersects      (const aabb& other) const noexcept { return {}; } //TODO
	std::optional<vec2f> circle::intersection    (const aabb& other) const noexcept { return {}; } //TODO
	bool                 circle::contains        (const aabb& other) const noexcept { return {}; } //TODO
	
	vec2f                circle::closest_point_to(const polygon& other) const noexcept { return {}; } //TODO
	vec2f                circle::distance_min    (const polygon& other) const noexcept { return {}; } //TODO
	bool                 circle::intersects      (const polygon& other) const noexcept { return {}; } //TODO
	std::optional<vec2f> circle::intersection    (const polygon& other) const noexcept { return {}; } //TODO
	bool                 circle::contains        (const polygon& other) const noexcept { return {}; } //TODO
	
	vec2f                circle::closest_point_to(const circle& other) const noexcept { return {}; } //TODO
	vec2f                circle::distance_min    (const circle& other) const noexcept { return {}; } //TODO
	bool                 circle::intersects      (const circle& other) const noexcept { return {}; } //TODO
	std::optional<vec2f> circle::intersection    (const circle& other) const noexcept { return {}; } //TODO
	bool                 circle::contains        (const circle& other) const noexcept { return {}; } //TODO
	}