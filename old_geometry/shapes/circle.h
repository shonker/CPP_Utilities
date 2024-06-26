#pragma once

#include "../common/begin.h"

namespace utils::math::geometry
	{
	struct circle : shape_base<circle>
		{
		public:
			vec2f center;
			float radius;

			inline aabb bounding_box() const noexcept
				{
				aabb ret
					{
					.ll{center.x - radius},
					.up{center.y - radius},
					.rr{center.x + radius},
					.dw{center.y + radius},
					};

				return ret;
				}
			
			using shape_base<circle>::closest_point_and_distance;
			using shape_base<circle>::closest_point_to;
			using shape_base<circle>::distance_min;
			using shape_base<circle>::vector_to;
			using shape_base<circle>::intersects;
			using shape_base<circle>::intersection_with;
			using shape_base<circle>::contains;
			using shape_base<circle>::collides_with;
			
			vec2f                closest_point_to(const point  & other) const noexcept;
			bool                 contains        (const point  & other) const noexcept;
			
			vec2f                closest_point_to(const segment& other) const noexcept;
			bool                 intersects      (const segment& other) const noexcept;
			std::optional<vec2f> intersection_with(const segment& other) const noexcept;
			bool                 contains        (const segment& other) const noexcept;
			
			vec2f                closest_point_to(const aabb   & other) const noexcept;
			vec2f                distance_min    (const aabb   & other) const noexcept;
			bool                 intersects      (const aabb   & other) const noexcept;
			std::optional<vec2f> intersection_with(const aabb   & other) const noexcept;
			bool                 contains        (const aabb   & other) const noexcept;
			
			vec2f                closest_point_to(const polygon& other) const noexcept;
			vec2f                distance_min    (const polygon& other) const noexcept;
			bool                 intersects      (const polygon& other) const noexcept;
			std::optional<vec2f> intersection_with(const polygon& other) const noexcept;
			bool                 contains        (const polygon& other) const noexcept;
			
			vec2f                closest_point_to(const circle & other) const noexcept;
			vec2f                distance_min    (const circle & other) const noexcept;
			bool                 intersects      (const circle & other) const noexcept;
			std::optional<vec2f> intersection_with(const circle & other) const noexcept;
			bool                 contains        (const circle & other) const noexcept;
			
			inline circle& scale_self    (const float      & scaling    ) noexcept { center.scale_self (scaling    ); radius *= scaling; return *this; };
			inline circle& rotate_self   (const angle::radf& rotation   ) noexcept { center.rotate_self(rotation   ); return *this; };
			inline circle& translate_self(const vec2f      & translation) noexcept { center.translate  (translation); return *this; };
		};
	}