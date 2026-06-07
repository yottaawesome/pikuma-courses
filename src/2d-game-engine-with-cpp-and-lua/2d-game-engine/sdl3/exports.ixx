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
		::SDL_Color,
		::SDL_PixelFormat,
		::SDL_Rect,
		::SDL_FRect,
		::SDL_Surface,
		::SDL_GetTicks,
		::SDL_DestroyTexture,
		::SDL_RenderTexture,
		::SDL_DestroySurface,
		::SDL_CreateTextureFromSurface,
		::SDL_RenderFillRect,
		::SDL_CreateWindowAndRenderer,
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

	using
		::IMG_Load
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
		Fullscreen = SDL_WINDOW_FULLSCREEN,
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

	[[deprecated("SDL_TICKS_PASSED macro was removed in SDL3, see migration guide at https://github.com/c-smile/SDL3/blob/main/docs/README-migration.md")]]
	inline constexpr auto TicksPassed(int64_t a, int64_t startTicks) noexcept -> bool
	{
		return a - startTicks <= 0;
	}
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
