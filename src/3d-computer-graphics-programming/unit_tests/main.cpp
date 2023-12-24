import std;
import shared;
import unit_tests;

template<typename... input_t>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<input_t>()...));

int main()
{
    using namespace unit_tests;
    
    testing::run(
        renderer_tests::buffer_2d_tests{},
        util_tests::sdl_context_tests{}
    );
    unit_tests::results::print_results();

    return 0;
}
