export module unit_tests:testing;
import std;
import std.compat;

export namespace unit_tests::testing
{
	template<typename T>
	struct test
	{
		std::string s;
		T run;
	};

	template<typename T>
	struct is_some_test : std::false_type {};

	template<typename T>
	struct is_some_test<test<T>> : std::true_type {};
	
	template<typename T>
	concept is_test = is_some_test<T>::value;

	template<is_test...T>
	constexpr std::tuple<T...> do_make_tests(T&&... t)
	{
		return std::tuple{ std::forward<T>(t)... };
	}

	template<typename T>
	struct is_some_tuple_of_tests : std::false_type {};

	template<typename...T>
	struct is_some_tuple_of_tests<std::tuple<T...>> : std::true_type {};

	template<typename T>
	concept is_tuple_of_tests = is_some_tuple_of_tests<T>::value;
}

std::tuple m{
	unit_tests::testing::test{"a", []() {}},
	unit_tests::testing::test{"a", []() {}}
};
static_assert(unit_tests::testing::is_tuple_of_tests<decltype(m)>);