export module unit_tests:buffer_2d;
import std;
import std.compat;
import renderer;
import :testing;

export namespace unit_tests::renderer
{
	struct renderer_tests
	{
		// with static ICEs
		unit_tests::testing::is_tuple_of_tests auto tests()
		{
			return unit_tests::testing::do_make_tests(
				unit_tests::testing::test{ "a", []() {} },
				unit_tests::testing::test{ "b", []() {} }
			);
		}
	};
}