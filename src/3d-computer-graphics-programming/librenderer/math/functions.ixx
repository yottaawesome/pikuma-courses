export module librenderer:math.functions;
import std;
import :concepts;

namespace
{
	constexpr auto sqrtNewtonRaphson(float x, float curr, float prev) -> float
	{
		return curr == prev
			? curr
			: sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
	}
}

export namespace math
{
	constexpr auto abs(concepts::is_arithmetic auto value)
	{
		if constexpr(std::unsigned_integral<decltype(value)>)
			return value;
		else
			return value >= 0 ? value : -value;
	}

	// Based on https://gist.github.com/alexshtf/eb5128b3e3e143187794.
	constexpr auto sqrt(float x) -> float
	{
		return x >= 0 and x < std::numeric_limits<float>::infinity()
			? sqrtNewtonRaphson(x, x, 0)
			: std::numeric_limits<float>::quiet_NaN();
	}
}
