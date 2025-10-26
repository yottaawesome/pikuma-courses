export module librenderer:math_functions;
import std;
import :concepts;

export namespace math
{
	constexpr auto abs(concepts::is_arithmetic auto value)
	{
		if constexpr(std::unsigned_integral<decltype(value)>)
			return value;
		else
			return value >= 0 ? value : -value;
	}
}
