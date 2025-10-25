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
		
		constexpr void normalise(this triangle& self) noexcept
		{
			for (auto& point : self.points)
				point.normalise();
		}

		constexpr auto compute_normal(this const triangle& self)
			noexcept -> vector_4f
		{
			const auto& [vector_a, vector_b, vector_c] = self.points;
			math::vector_4f vector_ab = vector_b - vector_a;
			math::vector_4f vector_ac = vector_c - vector_a;
			math::vector_4f ab_cross_ac =
				math::cross_product(vector_ab, vector_ac);
			ab_cross_ac.normalise();
			return ab_cross_ac;
		}
	};
}
