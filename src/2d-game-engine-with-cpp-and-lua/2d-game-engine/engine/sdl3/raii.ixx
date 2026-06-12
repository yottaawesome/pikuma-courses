export module engine:sdl.raii;
import std;
import :raii;
import :sdl3.exports;
import :sdl3.error;

namespace SDL
{
	struct SdlScope
	{
		SdlScope(std::uint32_t flags)
		{
			if (not SDL_Init(flags))
				throw SdlError{ "Failed to initialize SDL" };
		}

		~SdlScope()
		{
			SDL_Quit();
		}

		SdlScope(const SdlScope&) = delete;
		auto operator=(const SdlScope&) -> SdlScope& = delete;
		SdlScope(SdlScope&&) = delete;
		auto operator=(SdlScope&&) -> SdlScope & = delete;
	};

	using RendererUniquePtr = Engine::UniquePtr<SDL::SDL_Renderer, SDL::SDL_DestroyRenderer>;
	using WindowUniquePtr = Engine::UniquePtr<SDL::SDL_Window, SDL::SDL_DestroyWindow>;
}
