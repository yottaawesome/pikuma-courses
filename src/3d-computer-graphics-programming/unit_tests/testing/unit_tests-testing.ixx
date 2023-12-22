export module unit_tests:testing;
import std;
import std.compat;

export namespace unit_tests::testing
{
	template<typename T>
	struct test
	{
		std::string name;
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

	template<typename T>
	concept has_tests = requires(T t)
	{
		{t.tests()} -> unit_tests::testing::is_tuple_of_tests;
	};
}

namespace unit_tests::results
{
	unsigned successes = 0;
	unsigned failures = 0;
	unsigned total = 0;

	export
	{
		void report_success(const std::string& name)
		{
			successes++;
			total++;
		}

		void report_failure(const std::string& name)
		{
			std::println("Test {} failed", name);
			failures++;
			total++;
		}

		void print_results(std::chrono::high_resolution_clock::duration& runtime)
		{
			std::println("----------------------------");
			std::println(
				"All tests completed in {} ({}).", 
				std::chrono::duration_cast<std::chrono::milliseconds>(runtime),
				std::chrono::duration_cast<std::chrono::nanoseconds>(runtime)
			);
			std::println("{}/{} succeeded ({} failed).", successes, total, failures);
			std::println("----------------------------");
		}
	}
}

export namespace unit_tests::assert
{
	inline void is_true(const bool condition, const std::source_location& loc = std::source_location::current())
	{
		if (not condition)
			throw std::runtime_error(
				std::format(
					"Assertion failed at {} in {}:{}.",
					loc.function_name(),
					loc.file_name(),
					loc.line()
				)
			);
	}
}

std::tuple m{
	unit_tests::testing::test{"a", []() {}},
	unit_tests::testing::test{"a", []() {}}
};
static_assert(unit_tests::testing::is_tuple_of_tests<decltype(m)>);