import std;
import shared;
import unit_tests;

int main()
{
    using namespace unit_tests;
    
    testing::run(
        util_tests::buffer_2d_tests{},
        util_tests::sdl_context_tests{}
    );
    unit_tests::results::print_results();

    return 0;
}
