import librenderer;
import unit_tests;

int main()
{
    unit_tests::testing::run(
        unit_tests::math_tests::primitive_tests{}
    );
    unit_tests::results::print_results();

    return 0;
}
