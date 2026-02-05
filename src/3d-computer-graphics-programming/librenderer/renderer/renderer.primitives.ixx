export module librenderer:renderer.primitives;
import :math;

export namespace renderer
{
	struct textured_vertex
	{
		math::vector_4f position;
		math::tex2_coordinates texcoords;
	};

	struct face
	{
		int a = 0;
		int b = 0;
		int c = 0;
		std::uint32_t color = 0xffadd8e6;
		math::tex2_coordinates a_uv = { 0.0f, 0.0f };
		math::tex2_coordinates b_uv = { 0.0f, 0.0f };
		math::tex2_coordinates c_uv = { 0.0f, 0.0f };
	};

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
