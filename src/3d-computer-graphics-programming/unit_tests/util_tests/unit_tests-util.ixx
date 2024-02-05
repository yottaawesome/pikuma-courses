export module unit_tests:util;
import util;
import shared;
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
						util::sdl_context sdl{sdl::sdl_init_everything};
						assert::is_true(sdl.successful());
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
						util::radians value{std::numbers::pi};
						auto converted = util::convert(value);
						assert::is_true(converted.value == 180.f);
					}
				},
				testing::test{
					"Test degrees to radians",
					[] {
						util::degrees value{180};
						auto converted = util::convert(value);
						assert::is_true(converted.value == 3.14159274f);
					}
				}
			);
		}
	};
	static_assert(testing::test_exporter<functions_tests>);
}