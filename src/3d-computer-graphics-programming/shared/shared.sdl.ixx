module;

#include <SDL2/SDL.h>

export module shared.sdl;

export namespace sdl
{
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
		::SDL_Renderer
		;

}