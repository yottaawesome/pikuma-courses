module;

#include <SDL2/SDL.h>

export module librenderer:sdl;
import std;
import :raii;

export namespace sdl
{
	constexpr auto sdl_init_everything = SDL_INIT_EVERYTHING;
	constexpr auto sdl_windowpos_centered = SDL_WINDOWPOS_CENTERED;
	
	using
		::SDL_Init,
		::SDL_CreateWindow,
		::SDL_CreateRenderer,
		::SDL_PollEvent,
		::SDL_SetRenderDrawColor,
		::SDL_RenderClear,
		::SDL_RenderCopy,
		::SDL_RenderPresent,
		::SDL_DestroyWindow,
		::SDL_DestroyRenderer,
		::SDL_GetError,
		::SDL_Quit,
		::SDL_CreateTexture,
		::SDL_DestroyTexture,
		::SDL_UpdateTexture,
		::SDL_GetCurrentDisplayMode,
		::SDL_GetDisplayMode,
		::SDL_SetWindowFullscreen,
		::SDL_GetTicks,
		::SDL_GetTicks64,
		::SDL_Delay
		;

	using
		::SDL_Window,
		::SDL_Renderer,
		::SDL_Texture,
		::SDL_Event,
		::SDL_EventType,
		::SDL_KeyCode,
		::SDL_Keycode,
		::SDL_WindowFlags,
		::SDL_PixelFormatEnum,
		::SDL_TextureAccess,
		::SDL_DisplayMode
		;
}

export namespace sdl
{
	using sdl_window_unique_ptr = std::unique_ptr<sdl::SDL_Window, raii::deleter<SDL_DestroyWindow>>;
	using sdl_renderer_unique_ptr = std::unique_ptr<sdl::SDL_Renderer, raii::deleter<SDL_DestroyRenderer>>;
	using sdl_texture_unique_ptr = std::unique_ptr<sdl::SDL_Texture, raii::deleter<SDL_DestroyTexture>>;

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