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
	constexpr auto fps = 60;
	constexpr auto frame_target_time = std::chrono::milliseconds { 1000 / fps };

	auto triangles_to_render = std::vector<renderer::triangle>{}; // renderer::mesh_faces.size()
	auto context = std::make_unique<sdl::sdl_context>(sdl::sdl_init_everything);

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

	auto main_buffer = renderer::color_buffer{ window_dimensions.width(), window_dimensions.height()};

	auto window =
		[] -> sdl::sdl_window_unique_ptr
		{
			// Create a window
			// https://wiki.libsdl.org/SDL2/SDL_CreateWindow
			auto window = sdl::sdl_window_unique_ptr(
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

	auto sdl_renderer =
		[](const sdl::sdl_window_unique_ptr& window) -> sdl::sdl_renderer_unique_ptr
		{
			// Create a SDL renderer
			// https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
			auto renderer = sdl::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window.get(), -1, 0));
			if (not renderer)
				throw std::runtime_error(util::print_last_error());
			return renderer;
		}(window);

	auto color_buffer_texture = sdl::sdl_texture_unique_ptr{
		sdl::SDL_CreateTexture(
			sdl_renderer.get(),
			sdl::SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,
			sdl::SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
			window_dimensions.width(),
			window_dimensions.height()
		)
	};

	auto cube_points = std::array<math::vector_3f, number_of_points>{};
	auto camera_position = math::vector_4f{};

	bool is_running =
		[](std::array<math::vector_3f, number_of_points>& cube_points, const sdl::sdl_window_unique_ptr& window)
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

	auto projected_cube_points = std::array<math::vector_2f, number_of_points>{};

	auto previous_frame_time = std::chrono::milliseconds{ 0 };
	auto elapsed = std::chrono::milliseconds{ 0 };

	auto f22_mesh = renderer::mesh{ "..\\assets\\f22.obj" };
	auto cube_mesh = renderer::mesh{ renderer::load_cube_mesh() };
		//{ "..\\assets\\cube.obj" };
	auto mesh_to_render = cube_mesh;

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
