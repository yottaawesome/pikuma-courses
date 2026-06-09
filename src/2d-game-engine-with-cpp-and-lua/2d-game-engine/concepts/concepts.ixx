export module engine:concepts;
import std;

export namespace Concepts
{
	template<typename T, typename...TArgs>
	concept OneOf = (std::same_as<std::remove_cvref_t<T>, TArgs> or ...);
}
