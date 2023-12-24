export module unit_tests:util;
import util;
import shared;
import :framework;

export namespace unit_tests::util_tests
{
	struct sdl_context_tests
	{
		auto tests() -> testing::is_tuple_of_tests auto
		{
			return testing::make_tests(
				testing::test{
					"Test basic constructor",
					[]{
						util::sdl_context sdl{sdl::sdl_init_everything};
						assert::is_true(sdl.successful());
					}
				}
			);
		}
	};
	static_assert(testing::has_tests<sdl_context_tests>);
}