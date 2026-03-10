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

	constexpr auto window_dimensions = sdl::window_dimensions{};
	auto main_buffer = renderer::color_buffer{ window_dimensions.width(), window_dimensions.height()};
	auto window = sdl::window{ 
		window_dimensions.width(), 
		window_dimensions.height(), 
		sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS 
	};

	auto sdl_renderer = sdl::renderer{ window.get(), -1, 0 };

	auto color_buffer_texture = sdl::texture{
		sdl_renderer.get(),
		sdl::SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,
		sdl::SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
		static_cast<int>(window.get_width()),
		static_cast<int>(window.get_height())
	};

	auto cube_points = 
		[] -> std::array<math::vector_3f, number_of_points>
		{
			auto points = std::array<math::vector_3f, number_of_points>{};
			unsigned count = 0;
			for (float x = -1; x <= 1; x += 0.25)
				for (float y = -1; y <= 1; y += 0.25)
					for (float z = -1; z <= 1; z += 0.25)
						points[count++] = math::vector_3f{ .x=x, .y=y, .z=z };
			return points;
		}();
		
	auto camera_position = math::vector_4f{};
	auto is_running = true;
	auto projected_cube_points = std::array<math::vector_2f, number_of_points>{};
	auto previous_frame_time = std::chrono::milliseconds{ 0 };
	auto elapsed = std::chrono::milliseconds{ 0 };

	//auto f22_mesh = renderer::mesh{ "..\\assets\\f22.obj" };
	auto cube_mesh = renderer::mesh{ 
		//renderer::load_cube_mesh() 
		"..\\assets\\cube.obj"
	};
	auto mesh_to_render = cube_mesh;

	auto render_settings = renderer::settings{};

	constexpr float z_near = 0.1f;
	constexpr float z_far = 100.f;

	const math::projective_perspective_divide_matrix proj_matrix(
		math::radians{ std::numbers::pi / 3 },
		(float)main_app::window_dimensions.width() / (float)main_app::window_dimensions.height(),
		z_near,
		z_far
	);
}
