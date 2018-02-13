#pragma once

namespace ares
{
	// 2D vector
	template<typename v_t>
	struct Point
	{
		using value_t = v_t;

		value_t x, y;
		
		constexpr Point(const Point<value_t>& rv) = default;

		constexpr Point(value_t x, value_t y) : x { x }, y { y } { }

		constexpr Point<value_t> operator+(const Point<value_t>& rv) const
		{
			return { x + rv.x, y + rv.y };
		}

		constexpr Point<value_t> operator-(const Point<value_t>& rv) const
		{
			return { x - rv.x, y - rv.y };
		}
	};
}

using APoint = ares::Point<int>;