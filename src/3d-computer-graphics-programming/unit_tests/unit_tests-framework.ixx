export module unit_tests:framework;
import std;
import std.compat;

namespace unit_tests::results
{
	unsigned successes = 0;
	unsigned failures = 0;
	unsigned total = 0;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
}

export namespace unit_tests::results
{
	void time_start()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	void time_end()
	{
		end = std::chrono::high_resolution_clock::now();
	}

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

	void print_results()
	{
		auto duration = end - start;
		std::println("yottaawesome's brand spanking new unit testing framework!");
		std::println("---------------------------------------------------------");
		std::println(
			"All tests completed in {}/{}/{}.",
			std::chrono::duration_cast<std::chrono::milliseconds>(duration),
			std::chrono::duration_cast<std::chrono::microseconds>(duration),
			std::chrono::duration_cast<std::chrono::nanoseconds>(duration)
		);
		std::println("{}/{} succeeded ({} failed).", successes, total, failures);
		std::println("---------------------------------------------------------");
	}
}

export namespace unit_tests::testing
{
	// Our basic test type
	template<std::invocable T>
	struct test
	{
		std::string name;
		T run;
	};

	// Helper templates for determining whether a given type conforms 
	// to the test concept
	template<typename T>
	struct is_some_test : std::false_type {};

	template<typename T>
	struct is_some_test<test<T>> : std::true_type {};
	
	template<typename T>
	concept is_test = is_some_test<T>::value;
	// end

	// Helper templates for determining whether a type is a tuple of tests
	template<typename T>
	struct is_some_tuple_of_tests : std::false_type {};

	template<is_test...T>
	struct is_some_tuple_of_tests<std::tuple<T...>> : std::true_type {};

	template<typename T>
	concept is_tuple_of_tests = is_some_tuple_of_tests<T>::value;

	template<typename T>
	concept has_tests = requires(T t)
	{
		{t.tests()} -> testing::is_tuple_of_tests;
	};
	// end

	template<is_test...T>
	constexpr std::tuple<T...> make_tests(T&&... t)
	{
		return std::tuple{ std::forward<T>(t)... };
	}

	void run(testing::has_tests auto&&...testers)
	{
		// Concatenate all tests into one tuple
		std::tuple all_tests = std::tuple_cat(testers.tests()...);
		using all_tests_t = decltype(all_tests);

		results::time_start();
		// Forward the all tests tuple on to this lambda and expand for each of its elements
		[]<typename TTuple, size_t...I>(TTuple&& test_tuple, std::index_sequence<I...>)
		{
			// Forward each element sequentially in the test_tuple to this lambda to run the test
			([](testing::is_test auto&& test)
			{
				try
				{
					test.run();
					results::report_success(test.name);
				}
				catch (...)
				{
					results::report_failure(test.name);
				}
			}
			(std::forward<std::tuple_element_t<I, all_tests_t>>(std::get<I>(test_tuple))), ...);
		}(
			std::forward<all_tests_t>(all_tests),
			std::make_index_sequence<std::tuple_size_v<all_tests_t>>{}
		);
		results::time_end();
	}
}

export namespace unit_tests::assert
{
	struct assert_error final : public std::runtime_error
	{
		template<typename...T>
		assert_error(const std::format_string<T...>& fmt, T&&...args)
			: std::runtime_error(std::format(fmt, std::forward<T>(args)...))
		{ }
	};

	inline void is_true(const bool condition, const std::source_location& loc = std::source_location::current())
	{
		if (not condition)
			throw assert_error(
				"Assertion failed at {} in {}:{}.",
				loc.function_name(),
				loc.file_name(),
				loc.line()
			);
	}
}

std::tuple m{
	unit_tests::testing::test{"a", []() {}},
	unit_tests::testing::test{"a", []() {}}
};
static_assert(unit_tests::testing::is_tuple_of_tests<decltype(m)>);