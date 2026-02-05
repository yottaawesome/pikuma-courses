export module librenderer:mainapp;
import std;
import :math;
import :renderer;
import :util;
import :math;
import :raii;

export namespace main_app
{
	constexpr auto number_of_points = 9 * 9 * 9;
	constexpr int fps = 60;
	constexpr std::chrono::milliseconds frame_target_time{ 1000 / fps };

	std::vector<math::triangle> triangles_to_render; // renderer::mesh_faces.size()

	std::unique_ptr<raii::sdl_context> context = std::make_unique<raii::sdl_context>(sdl::sdl_init_everything);

	struct window_dimension_t 
	{
		auto width(this const auto&) noexcept -> std::uint32_t
		{
			static std::uint32_t value = 
				[] static
				{
					sdl::SDL_DisplayMode mode;
					// https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
					if (sdl::SDL_GetCurrentDisplayMode(0, &mode))
						throw std::runtime_error(util::print_last_error());
					return static_cast<uint32_t>(mode.w) - 100;
				}();
			return value;
		}
		auto height(this const auto&) noexcept -> std::uint32_t
		{
			static std::uint32_t value =
				[] static
				{
					sdl::SDL_DisplayMode mode;
					// https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
					if (sdl::SDL_GetCurrentDisplayMode(0, &mode))
						throw std::runtime_error(util::print_last_error());
					return static_cast<uint32_t>(mode.h) - 100;
				}();
			return value;
		}
	} constexpr window_dimensions;

	math::color_buffer main_buffer =
		{ window_dimensions.width(), window_dimensions.height()};

	raii::sdl_window_unique_ptr window =
		[]
		{
			// Create a window
			// https://wiki.libsdl.org/SDL2/SDL_CreateWindow
			auto window = raii::sdl_window_unique_ptr(
				sdl::SDL_CreateWindow(
					nullptr,
					sdl::sdl_windowpos_centered,
					sdl::sdl_windowpos_centered,
					window_dimensions.width(),
					window_dimensions.height(),
					sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS
				)
			);
			if (not window)
				throw std::runtime_error(util::print_last_error());
			return window;
		}();

	raii::sdl_renderer_unique_ptr sdl_renderer =
		[](const raii::sdl_window_unique_ptr& window)
		{
			// Create a SDL renderer
			// https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
			auto renderer = raii::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window.get(), -1, 0));
			if (not renderer)
				throw std::runtime_error(util::print_last_error());
			return renderer;
		}(window);

	raii::sdl_texture_unique_ptr color_buffer_texture =
		[]
		{
			return raii::sdl_texture_unique_ptr{
				sdl::SDL_CreateTexture(
					sdl_renderer.get(),
					sdl::SDL_PixelFormatEnum::SDL_PIXELFORMAT_ARGB8888,
					sdl::SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
					window_dimensions.width(),
					window_dimensions.height()
				)
			};
		}();

	std::array<math::vector_3f, number_of_points> cube_points{};
	math::vector_3f camera_position;

	bool is_running =
		[](std::array<math::vector_3f, number_of_points>& cube_points, const raii::sdl_window_unique_ptr& window)
		{
			unsigned count = 0;
			for (float x = -1; x <= 1; x += 0.25)
				for (float y = -1; y <= 1; y += 0.25)
					for (float z = -1; z <= 1; z += 0.25)
						cube_points[count++] = math::vector_3f{ .x=x, .y=y, .z=z };

			//if (sdl::SDL_SetWindowFullscreen(window.get(), sdl::SDL_WindowFlags::SDL_WINDOW_FULLSCREEN))
				//throw std::runtime_error(util::print_last_error());

			return true;
		}(cube_points, window);

	std::array<math::vector_2f, number_of_points> projected_cube_points{};

	std::chrono::milliseconds previous_frame_time{ 0 };
	std::chrono::milliseconds elapsed{ 0 };

	renderer::mesh f22_mesh{ "..\\assets\\f22.obj" };
	renderer::mesh cube_mesh
		{ renderer::load_cube_mesh() };
		//{ "..\\assets\\cube.obj" };

	renderer::mesh mesh_to_render = cube_mesh;

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

	struct renderer_settings
	{
		render_mode rendering_mode = render_mode::filled_wireframe;
		cull_mode culling_mode = cull_mode::enabled;
		auto should_draw_filled_triangles(this const renderer_settings& self) -> bool
		{
			return self.rendering_mode == render_mode::filled
				or self.rendering_mode == render_mode::filled_wireframe;
		}

		auto should_draw_triangles(this const renderer_settings& self) -> bool
		{
			return self.rendering_mode == main_app::render_mode::filled_wireframe
				or self.rendering_mode == main_app::render_mode::wireframe
				or self.rendering_mode == main_app::render_mode::wireframe_with_dot
				or self.rendering_mode == main_app::render_mode::textured_wireframe;
		}

		auto should_draw_points(this const renderer_settings& self) -> bool
		{
			return self.rendering_mode == main_app::render_mode::wireframe_with_dot;
		}

		auto should_draw_textured_triangles(this const renderer_settings& self) -> bool
		{
			return self.rendering_mode == main_app::render_mode::textured
				or self.rendering_mode == main_app::render_mode::textured_wireframe;
		}
	} render_settings;


	const math::projective_perspective_divide_matrix proj_matrix(
		math::radians{ std::numbers::pi / 3 },
		(float)main_app::window_dimensions.width() / (float)main_app::window_dimensions.height(),
		0.1f,
		100.f
	);
}
