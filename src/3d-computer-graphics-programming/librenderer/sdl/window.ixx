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
					SDL_DisplayMode mode;
					// https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
					if (SDL_GetCurrentDisplayMode(0, &mode))
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
					SDL_DisplayMode mode;
					// https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
					if (SDL_GetCurrentDisplayMode(0, &mode))
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
		window(std::uint32_t width, std::uint32_t height, SDL_WindowFlags flags)
			: sdlWindow{ create(width, height, flags) }
		{ }

		constexpr auto get(this const window& self) noexcept -> SDL_Window* 
		{ 
			return self.sdlWindow.get(); 
		}

		static auto create(
			std::uint32_t width, 
			std::uint32_t height, 
			SDL_WindowFlags flags
		) -> sdl::window_unique_ptr
		{
			// Create a window
			// https://wiki.libsdl.org/SDL2/SDL_CreateWindow
			auto window = sdl::window_unique_ptr(
				SDL_CreateWindow(
					nullptr,
					sdl::windowpos_centered,
					sdl::windowpos_centered,
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
			SDL_GetWindowSize(self.get(), &width, nullptr);
			return static_cast<std::uint32_t>(width);
		}

		auto get_height(this const window& self) noexcept -> std::uint32_t
		{
			int height;
			SDL_GetWindowSize(self.get(), nullptr, &height);
			return static_cast<std::uint32_t>(height);
		}

		auto get_dimensions(this const window& self) noexcept -> dimensions
		{
			int width, height;
			SDL_GetWindowSize(self.get(), &width, &height);
			return { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
		}

		void set_fullscreen(this window& self, bool should_be_fullscreen)
		{
			if (SDL_SetWindowFullscreen(self.get(), should_be_fullscreen ? SDL_WindowFlags::SDL_WINDOW_FULLSCREEN : 0))
				throw std::runtime_error(sdl::print_last_error());
		}
		
	private:
		sdl::window_unique_ptr sdlWindow;
	};
}
