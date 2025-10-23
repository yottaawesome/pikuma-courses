export module librenderer:primitives;
import std;
import :vector;

export namespace math
{
	struct point_2d final
	{
		std::uint32_t x = 0;
		std::uint32_t y = 0;
	};

	struct basic_rectangle
	{
		std::uint32_t width = 0;
		std::uint32_t height = 0;
	};

	struct rectangle : basic_rectangle
	{
		std::uint32_t x = 0;
		std::uint32_t y = 0;
		auto is_in_point_space(this const rectangle& self, point_2d point) noexcept -> bool
		{
			return (self.x + self.width < point.x) and (self.y + self.height < point.y);
		}
	};

	struct face
	{
		int a = 0;
		int b = 0;
		int c = 0;
		std::uint32_t color = 0xffadd8e6;
	};

	struct triangle
	{
		vector_4f points[3];
		std::uint32_t color = 0xffffffff;
		float average_depth = 0; // for painter's algorithm
	};
}
