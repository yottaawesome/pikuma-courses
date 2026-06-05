module;

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

export module engine:sdl3.exports;

export namespace SDL
{
	using
		::SDL_Window,
		::SDL_Renderer,
		::SDL_Event,
		::SDL_EventType,
		::SDL_SetWindowPosition,
		::SDL_RenderClear,
		::SDL_RenderPresent,
		::SDL_SetRenderDrawColor,
		::SDL_PollEvent,
		::SDL_DestroyRenderer,
		::SDL_DestroyWindow,
		::SDL_Quit,
		::SDL_CreateRenderer,
		::SDL_Init,
		::SDL_CreateWindow,
		::SDL_GetError
		;

	enum InitFlags : Uint32
	{
		Audio = SDL_INIT_AUDIO,
		Video = SDL_INIT_VIDEO,
	};

	using WindowFlags = SDL_WindowFlags;

	enum Windowpos
	{
		Undefined = SDL_WINDOWPOS_UNDEFINED,
		Centered = SDL_WINDOWPOS_CENTERED,
	};

	enum Window
	{
		Borderless = SDL_WINDOW_BORDERLESS,
	};

	enum EventType
	{
		Quit = SDL_EVENT_QUIT,
		KeyDown = SDL_EVENT_KEY_DOWN,
	};

	enum Scancode
	{
		Escape = SDL_SCANCODE_ESCAPE,
	};
}

namespace SDL::Mixer
{
}

namespace SDL::TTF
{
}

namespace SDL::Image
{
}
