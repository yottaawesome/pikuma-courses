import std;
import shared;
import unit_tests;

template<typename ... input_t>
using tuple_cat_t =
decltype(std::tuple_cat(
    std::declval<input_t>()...
));

template<typename T>
concept has_tests = requires(T t)
{
    {t.tests()} -> unit_tests::testing::is_tuple_of_tests;
};

void Run(has_tests auto&&...s)
{
    std::tuple all_tests = std::tuple_cat(s.tests()...);
    using all_tests_t = decltype(all_tests);

    []<typename TTuple, size_t...I>(TTuple&& t, std::index_sequence<I...>)
    {
        ([](unit_tests::testing::is_test auto&& test)
        {    
            try
            {
                test.run();
            }
            catch (...)
            {

            }
        }
        (std::forward<std::tuple_element_t<I, all_tests_t>>(std::get<I>(t))), ...);
    }(
        std::forward<all_tests_t>(all_tests),
        std::make_index_sequence<std::tuple_size_v<all_tests_t>>{}
    );
}

int main()
{
    Run(unit_tests::renderer::renderer_tests{}, unit_tests::renderer::renderer_tests{});

    return 0;
}
