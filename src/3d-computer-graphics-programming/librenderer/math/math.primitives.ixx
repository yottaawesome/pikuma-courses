export module librenderer:math.primitives;
import std;
import :math.vector;
import :util;
import :renderer.texture;

export namespace math
{
	struct tex2_coordinates
	{
		float u = 0.f;
		float v = 0.f;
	};

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
}
