export module unit_tests:util;
import librenderer;
import :framework;

export namespace unit_tests::util_tests
{
	struct sdl_context_tests
	{
		auto tests() -> testing::testables auto
		{
			return testing::make_tests(
				testing::test{
					"Test basic constructor",
					[]{
						raii::sdl_context sdl{sdl::sdl_init_everything};
					}
				}
			);
		}
	};
	static_assert(testing::test_exporter<sdl_context_tests>);
}