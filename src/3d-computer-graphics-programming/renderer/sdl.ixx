module;

#include <SDL2/SDL.h>

export module sdl;

export namespace SDL
{
	using 
		::SDL_Init,
		::SDL_CreateWindow,
		::SDL_CreateRenderer,
		::SDL_PollEvent,
		::SDL_SetRenderDrawColor,
		::SDL_RenderClear,
		::SDL_RenderPresent
		;

}