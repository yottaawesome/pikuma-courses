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

	struct functions_tests
	{
		auto tests() -> testing::testables auto
		{
			return testing::make_tests(
				testing::test{
					"Test radians to degrees",
					[] {
						math::radians value{std::numbers::pi};
						auto converted = math::convert(value);
						assert::is_true(converted == 180.f);
					}
				},
				testing::test{
					"Test degrees to radians",
					[] {
						math::degrees value{180};
						auto converted = math::convert(value);
						assert::is_true(converted == 3.14159274f);
					}
				}
			);
		}
	};
	static_assert(testing::test_exporter<functions_tests>);
}
