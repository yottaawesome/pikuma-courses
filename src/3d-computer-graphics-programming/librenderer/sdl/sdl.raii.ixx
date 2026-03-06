export module librenderer:sdl.raii;
import std;
import :raii;
import :sdl.exports;

export namespace sdl
{
	using sdl_window_unique_ptr = raii::direct_unique_ptr<sdl::SDL_Window, SDL_DestroyWindow>;
	using sdl_renderer_unique_ptr = raii::direct_unique_ptr<sdl::SDL_Renderer, SDL_DestroyRenderer>;
	using sdl_texture_unique_ptr = raii::direct_unique_ptr<sdl::SDL_Texture, SDL_DestroyTexture>;

	struct sdl_context final
	{
		~sdl_context()
		{
			sdl::SDL_Quit();
		}

		sdl_context(const sdl_context&) = delete;
		sdl_context& operator=(sdl_context&&) = delete;

		sdl_context(int flags)
		{
			if (sdl::SDL_Init(sdl::sdl_init_everything) != 0)
				throw std::runtime_error("Failed initialisation of SDL2.");
		}
	};
}
