module;

#include <SDL2/SDL.h>

export module librenderer:lib.sdl;

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