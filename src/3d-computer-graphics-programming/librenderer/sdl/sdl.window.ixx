export module librenderer:sdl.window;
import std;
import :sdl.exports;
import :sdl.error;
import :sdl.raii;

export namespace sdl
{
	struct window_dimensions
	{
		auto width(this const auto&) noexcept -> std::uint32_t
		{
			static std::uint32_t value =
				[] static
				{
					sdl::SDL_DisplayMode mode;
					// https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
					if (sdl::SDL_GetCurrentDisplayMode(0, &mode))
						throw std::runtime_error(print_last_error());
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
						throw std::runtime_error(print_last_error());
					return static_cast<uint32_t>(mode.h) - 100;
				}();
			return value;
		}
	};

	class window final
	{
	public:
		struct dimensions
		{
			std::uint32_t width = 0;
			std::uint32_t height = 0;
		};

		constexpr window() = default;
		window(std::uint32_t width, std::uint32_t height, sdl::SDL_WindowFlags flags)
			: sdlWindow{ create(width, height, flags) }
		{ }

		constexpr auto get(this const window& self) noexcept -> SDL_Window* 
		{ 
			return self.sdlWindow.get(); 
		}

		static auto create(
			std::uint32_t width, 
			std::uint32_t height, 
			sdl::SDL_WindowFlags flags
		) -> sdl::sdl_window_unique_ptr
		{
			// Create a window
			// https://wiki.libsdl.org/SDL2/SDL_CreateWindow
			auto window = sdl::sdl_window_unique_ptr(
				sdl::SDL_CreateWindow(
					nullptr,
					sdl::sdl_windowpos_centered,
					sdl::sdl_windowpos_centered,
					width,
					height,
					flags
				)
			);
			if (not window)
				throw std::runtime_error(sdl::print_last_error());
			return window;
		}

		auto get_width(this const window& self) noexcept -> std::uint32_t
		{
			int width;
			sdl::SDL_GetWindowSize(self.get(), &width, nullptr);
			return static_cast<std::uint32_t>(width);
		}

		auto get_height(this const window& self) noexcept -> std::uint32_t
		{
			int height;
			sdl::SDL_GetWindowSize(self.get(), nullptr, &height);
			return static_cast<std::uint32_t>(height);
		}

		auto get_dimensions(this const window& self) noexcept -> dimensions
		{
			int width, height;
			sdl::SDL_GetWindowSize(self.get(), &width, &height);
			return { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
		}

		void set_fullscreen(this window& self, bool should_be_fullscreen)
		{
			if (sdl::SDL_SetWindowFullscreen(self.get(), should_be_fullscreen ? sdl::SDL_WindowFlags::SDL_WINDOW_FULLSCREEN : 0))
				throw std::runtime_error(sdl::print_last_error());
		}
		
	private:
		sdl::sdl_window_unique_ptr sdlWindow;
	};

	class renderer final
	{
	public:
		renderer(sdl::SDL_Window* window, int index, uint32_t flags)
			: sdlRenderer{ create(window, index, flags) }
		{ }

		static auto create(
			sdl::SDL_Window* window, 
			int index, 
			uint32_t flags
		) -> sdl::sdl_renderer_unique_ptr
		{
			// Create a SDL renderer
			// https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
			auto renderer = sdl::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window, index, flags));
			if (not renderer)
				throw std::runtime_error(sdl::print_last_error());
			return renderer;
		}

		constexpr auto get(this const renderer& self) noexcept -> SDL_Renderer*
		{
			return self.sdlRenderer.get();
		}
	private:
		sdl::sdl_renderer_unique_ptr sdlRenderer;
	};

	class texture final
	{
	public:
		texture(
			sdl::SDL_Renderer* renderer,
			uint32_t format,
			int access,
			int width,
			int height
		) : sdlTexture{ create(renderer, format, access, width, height) }
		{ }

		static auto create(
			sdl::SDL_Renderer* renderer,
			uint32_t format,
			int access,
			int width,
			int height
		) -> sdl::sdl_texture_unique_ptr
		{
			// Create a SDL texture
			// https://wiki.libsdl.org/SDL2/SDL_CreateTexture
			auto texture = sdl::sdl_texture_unique_ptr(
				sdl::SDL_CreateTexture(renderer, format, access, width, height)
			);
			if (not texture)
				throw std::runtime_error(sdl::print_last_error());
			return texture;
		}

		constexpr auto get(this const texture& self) noexcept -> SDL_Texture*
		{
			return self.sdlTexture.get();
		}
	private:
		sdl::sdl_texture_unique_ptr sdlTexture;
	};
}
