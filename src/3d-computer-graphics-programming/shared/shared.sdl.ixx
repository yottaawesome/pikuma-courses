module;

#include <SDL2/SDL.h>

export module shared.sdl;

export namespace sdl
{
	constexpr auto sdl_init_everything = SDL_INIT_EVERYTHING;
	constexpr auto sdl_windowpos_centered = SDL_WINDOWPOS_CENTERED;
	using ::SDL_WindowFlags;

	using 
		::SDL_Init,
		::SDL_CreateWindow,
		::SDL_CreateRenderer,
		::SDL_PollEvent,
		::SDL_SetRenderDrawColor,
		::SDL_RenderClear,
		::SDL_RenderPresent,
		::SDL_DestroyWindow,
		::SDL_DestroyRenderer,
		::SDL_GetError
		;

	using
		::SDL_Window,
		::SDL_Renderer,
		::SDL_Event,
		::SDL_EventType,
		::SDL_KeyCode
		;
}