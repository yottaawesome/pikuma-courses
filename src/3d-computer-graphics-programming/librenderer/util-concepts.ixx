export module util:concepts;
import std;

// for general utility concepts and templates
export namespace util::concepts
{
	template<typename T>
	struct false_type : std::false_type {};

	template<typename TRequired, typename...TToTest>
	concept one_of = (std::same_as<TRequired, TToTest> or ...);

	template<typename TRequired, typename...TToTest>
	concept all_of = (std::same_as<TRequired, TToTest> and ...);

	enum class tuple_for_each_behaviour
	{
		as_and,
		as_or,
		as_all
	};

	template<tuple_for_each_behaviour TBehaviour>
	auto tuple_for_each(auto&& tuple, auto&& func)
	{
		using TTuple = std::remove_cvref_t<decltype(tuple)>;
		using TFunc = std::remove_cvref_t<decltype(func)>;
		return []<size_t...Is>(auto&& tuple, auto&& func, std::index_sequence<Is...>)
		{
			if constexpr (TBehaviour == tuple_for_each_behaviour::as_and)
				return (std::invoke(func, std::get<Is>(tuple)) and ...);

			else if constexpr (TBehaviour == tuple_for_each_behaviour::as_or)
				return (std::invoke(func, std::get<Is>(tuple)) or ...);

			else if constexpr (TBehaviour == tuple_for_each_behaviour::as_all)
				return (std::invoke(func, std::get<Is>(tuple)), ...);
		}(
			std::forward<TTuple>(tuple), 
			std::forward<TFunc>(func), 
			std::make_index_sequence<TTuple>{}
		);
	}
}
