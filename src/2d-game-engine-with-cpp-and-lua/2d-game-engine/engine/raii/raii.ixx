export module engine:raii;
import std;

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

	template<auto Fn>
	struct Finally
	{
		constexpr ~Finally() { Fn(); }
	};
}
