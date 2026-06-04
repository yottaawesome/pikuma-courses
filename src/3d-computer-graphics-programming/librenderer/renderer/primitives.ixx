export module renderer:renderer.primitives;
import :math;

export namespace renderer
{
	struct tex2_coordinates
	{
		float u = 0.f;
		float v = 0.f;
	};

	struct textured_vertex
	{
		vector_4f position;
		tex2_coordinates texcoords;
	};

	struct face
	{
		int a = 0;
		int b = 0;
		int c = 0;
		std::uint32_t color = 0xffadd8e6;
		tex2_coordinates a_uv = { 0.0f, 0.0f };
		tex2_coordinates b_uv = { 0.0f, 0.0f };
		tex2_coordinates c_uv = { 0.0f, 0.0f };
	};

	struct triangle
	{
		vector_4f vertices[3];
		tex2_coordinates texcoords[3];
		std::uint32_t color = 0xffffffff;

		constexpr void normalise(this triangle& self) noexcept
		{
			for (auto& point : self.vertices)
				::renderer::normalise(point);
		}

		constexpr auto compute_normal(this const triangle& self)
			noexcept -> vector_4f
		{
			const auto& [vert_a, vert_b, vert_c] = self.vertices;
			vector_4f vector_ab = vert_b - vert_a; // A -> B
			//vector_ab.normalise();
			vector_4f vector_ac = vert_c - vert_a; // A -> C
			//vector_ac.normalise();
			//vector_4f vector_bc = vert_c - vert_b; // B -> C
			vector_4f normal = cross_product(vector_ab, vector_ac);
			::renderer::normalise(normal);
			if not consteval
			{
				/*auto mag = normal.magnitude();
					util::print_debug_string("Normal magnitude: {}", mag);*/
			}
			return normal;
		}
	};
}
