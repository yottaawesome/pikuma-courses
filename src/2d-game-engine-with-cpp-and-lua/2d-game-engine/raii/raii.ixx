export module engine:raii;
import std;
import :sdl3;

namespace Engine
{
	template<auto Fn>
	struct Deleter
	{
		static constexpr auto operator()(auto ptr)
		{
			Fn(ptr);
		}
	};
	template<typename T, auto Func>
	using UniquePtr = std::unique_ptr<T, Deleter<Func>>;

	using RendererUniquePtr = UniquePtr<SDL::SDL_Renderer, SDL::SDL_DestroyRenderer>;
	using WindowUniquePtr = UniquePtr<SDL::SDL_Window, SDL::SDL_DestroyWindow>;

	template<auto Fn>
	struct Finally
	{
		constexpr ~Finally() { Fn(); }
	};
}
