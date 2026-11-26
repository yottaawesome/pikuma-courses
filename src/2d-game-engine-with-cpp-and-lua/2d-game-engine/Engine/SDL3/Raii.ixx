export module Engine:SDL.Raii;
import std;
import :Raii;
import :SDL3.Exports;
import :SDL3.Error;

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
