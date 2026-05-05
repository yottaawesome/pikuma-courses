export module librenderer:math.primitives;
import std;

export namespace math
{
	struct point_2d
	{
		std::uint32_t x = 0;
		std::uint32_t y = 0;
	};

	struct rectangle
	{
		std::uint32_t width = 0;
		std::uint32_t height = 0;
		std::uint32_t x = 0;
		std::uint32_t y = 0;
		auto is_in_point_space(this const rectangle& self, point_2d point) noexcept -> bool
		{
			return (self.x + self.width < point.x) and (self.y + self.height < point.y);
		}
	};
}
