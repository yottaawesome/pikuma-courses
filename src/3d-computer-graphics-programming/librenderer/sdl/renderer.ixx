export module librenderer:sdl.renderer;
import std;
import :sdl.error;
import :sdl.raii;
import :sdl.exports;

export namespace sdl
{
	class renderer final
	{
	public:
		renderer(sdl::SDL_Window* window, int index, std::uint32_t flags)
			: sdlRenderer{ create(window, index, flags) }
		{ }

		static auto create(
			sdl::SDL_Window* window,
			int index,
			std::uint32_t flags
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
}
