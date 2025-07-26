export module unit_tests;
export import :util;
export import :framework;
export import :primitives;

export extern "C" int main()
{
    unit_tests::testing::run(
        unit_tests::math_tests::primitive_tests{}
    );
    unit_tests::results::print_results();

    return 0;
}
