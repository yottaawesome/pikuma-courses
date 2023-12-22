import std;
import shared;
import unit_tests;

template<typename ... input_t>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<input_t>()...));

int main()
{
    std::chrono::high_resolution_clock::duration runtime = unit_tests::testing::run(
        unit_tests::renderer::renderer_tests{}
    );
    unit_tests::results::print_results(runtime);

    return 0;
}
