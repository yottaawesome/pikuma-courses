import librenderer;
import unit_tests;

int main()
{
    using namespace unit_tests;
    
    testing::run(
        util_tests::buffer_2d_tests{},
        util_tests::sdl_context_tests{},
        util_tests::functions_tests{},
        util_tests::primitive_tests{}
    );
    unit_tests::results::print_results();

    return 0;
}
