export module librenderer:renderer.primitives;
import :math;

export namespace renderer
{
	struct triangle
	{
		math::vector_4f vertices[3];
		math::tex2_coordinates texcoords[3];
		std::uint32_t color = 0xffffffff;
		float average_depth = 0; // for painter's algorithm

		constexpr void normalise(this triangle& self) noexcept
		{
			for (auto& point : self.vertices)
				point.normalise();
		}

		constexpr auto compute_normal(this const triangle& self)
			noexcept -> math::vector_4f
		{
			const auto& [vert_a, vert_b, vert_c] = self.vertices;
			math::vector_4f vector_ab = vert_b - vert_a; // A -> B
			//vector_ab.normalise();
			math::vector_4f vector_ac = vert_c - vert_a; // A -> C
			//vector_ac.normalise();
			//math::vector_4f vector_bc = vert_c - vert_b; // B -> C
			math::vector_4f normal = math::cross_product(vector_ab, vector_ac);
			normal.normalise();
			if not consteval
			{
				/*auto mag = normal.magnitude();
					util::print_debug_string("Normal magnitude: {}", mag);*/
			}
			return normal;
		}
	};
}
