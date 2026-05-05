export module librenderer:sdl.raii;
import std;
import :raii;
import :sdl.exports;

export namespace sdl
{
	using window_unique_ptr = raii::direct_unique_ptr<SDL_Window, SDL_DestroyWindow>;
	using renderer_unique_ptr = raii::direct_unique_ptr<SDL_Renderer, SDL_DestroyRenderer>;
	using texture_unique_ptr = raii::direct_unique_ptr<SDL_Texture, SDL_DestroyTexture>;

	struct sdl_context final
	{
		~sdl_context()
		{
			SDL_Quit();
		}

		sdl_context(const sdl_context&) = delete;
		sdl_context& operator=(sdl_context&&) = delete;

		sdl_context(int flags)
		{
			if (SDL_Init(sdl::init_everything) != 0)
				throw std::runtime_error("Failed initialisation of SDL2.");
		}
	};
}
