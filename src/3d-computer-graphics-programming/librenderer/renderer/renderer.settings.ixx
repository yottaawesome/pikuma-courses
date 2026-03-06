export module librenderer:renderer.settings;

export namespace renderer
{
	enum class render_mode
	{
		wireframe_with_dot,
		wireframe,
		filled,
		filled_wireframe,
		textured,
		textured_wireframe
	};
	enum class cull_mode
	{
		enabled,
		disabled
	};

	struct settings
	{
		render_mode rendering_mode = render_mode::filled_wireframe;
		cull_mode culling_mode = cull_mode::enabled;
		auto should_draw_filled_triangles(this const settings& self) -> bool
		{
			return self.rendering_mode == render_mode::filled
				or self.rendering_mode == render_mode::filled_wireframe;
		}

		auto should_draw_triangles(this const settings& self) -> bool
		{
			return self.rendering_mode == render_mode::filled_wireframe
				or self.rendering_mode == render_mode::wireframe
				or self.rendering_mode == render_mode::wireframe_with_dot
				or self.rendering_mode == render_mode::textured_wireframe;
		}

		auto should_draw_points(this const settings& self) -> bool
		{
			return self.rendering_mode == render_mode::wireframe_with_dot;
		}

		auto should_draw_textured_triangles(this const settings& self) -> bool
		{
			return self.rendering_mode == render_mode::textured
				or self.rendering_mode == render_mode::textured_wireframe;
		}
	};
}
