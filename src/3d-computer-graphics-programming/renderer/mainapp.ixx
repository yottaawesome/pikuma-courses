export module mainapp;
import std;
import librenderer;

export namespace app_state
{
	struct mesh_and_texture
	{
		mesh_and_texture(std::string_view mesh_path, std::string_view texture_path)
			: mesh{ mesh_path }, texture{ texture_path }
		{}
		renderer::mesh mesh{ "..\\assets\\f22.obj" };
		upng::upng_texture texture{ "..\\assets\\f22_texture.png" };
	};

	struct all_meshes_t
	{
		std::array<mesh_and_texture, 5> meshes = std::array{
			mesh_and_texture{ "..\\assets\\f117.obj", "..\\assets\\f117.png" },
			mesh_and_texture{ "..\\assets\\f22.obj", "..\\assets\\f22.png" },
			mesh_and_texture{ "..\\assets\\cube.obj", "..\\assets\\cube.png" },
			mesh_and_texture{ "..\\assets\\drone.obj", "..\\assets\\drone.png" },
			mesh_and_texture{ "..\\assets\\efa.obj", "..\\assets\\efa.png" },
		};
		std::size_t current_mesh_index = 0;
		constexpr auto operator++(this auto&& self) noexcept -> decltype(auto)
		{
			self.current_mesh_index = (self.current_mesh_index + 1) % self.meshes.size();
			return decltype(self)(self);
		}
		constexpr auto operator++(this auto&& self, int) noexcept -> decltype(auto)
		{
			self.current_mesh_index = (self.current_mesh_index + 1) % self.meshes.size();
			return decltype(self)(self);
		}
		constexpr auto operator--(this auto&& self) noexcept -> decltype(auto)
		{
			self.current_mesh_index = (self.current_mesh_index + self.meshes.size() - 1) % self.meshes.size();
			return decltype(self)(self);
		}
		constexpr auto operator--(this auto&& self, int) noexcept -> decltype(auto)
		{
			self.current_mesh_index = (self.current_mesh_index + self.meshes.size() - 1) % self.meshes.size();
			return decltype(self)(self);
		}
		inline auto get_current_mesh(this auto&& self) noexcept -> decltype(auto)
		{
			return std::forward_like<decltype(self)>(self.meshes)[self.current_mesh_index];
		}
		auto for_each(this auto&& self, auto&& func) -> decltype(auto)
		{
			for (auto&& mesh : self.meshes)
				func(mesh);
			return std::forward_like<decltype(self)>(self);
		}
	};
	auto all_meshes = all_meshes_t{};

	constexpr auto fps = 60;
	constexpr auto frame_target_time = std::chrono::milliseconds{ 1000 / fps };
	auto previous_frame_time = std::chrono::milliseconds{ 0 };
	auto elapsed = std::chrono::milliseconds{ 0 };

	auto triangles_to_render = std::vector<renderer::triangle>{}; // renderer::mesh_faces.size()
	auto context = std::make_unique<sdl::sdl_context>(sdl::sdl_init_everything);

	constexpr auto window_dimensions = sdl::window_dimensions{};
	
	// combines the color and depth buffer into a single struct.
	auto frame_buffer = renderer::frame_buffer{ window_dimensions.width(), window_dimensions.height() };

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

	auto camera_position = math::vector_4f{};
	auto is_running = true;
	auto render_settings = renderer::settings{};

	const auto proj_matrix = math::projective_perspective_divide_matrix(
		math::radians{ std::numbers::pi / 3 },
		(float)window_dimensions.width() / (float)window_dimensions.height(),
		0.1f,
		100.f
	);
}
