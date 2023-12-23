export module unit_tests:buffer_2d;
import std;
import std.compat;
import renderer;
import :framework;

export namespace unit_tests::renderer
{
	struct renderer_tests
	{
		// MSVC ICEs when this is made static
		auto tests() -> testing::is_tuple_of_tests auto
		{
			return unit_tests::testing::make_tests(
				unit_tests::testing::test{ 
					"Test default constructor", 
					[]() 
					{
						buffer_2d<uint32_t> buffer;
						assert::is_true(buffer.width() == 0);
						assert::is_true(buffer.height() == 0);
					} 
				},

				unit_tests::testing::test{ 
					"Test width/height constructor", 
					[]() 
					{
						buffer_2d<uint32_t> buffer{200, 300};
						assert::is_true(buffer.width() == 200);
						assert::is_true(buffer.height() == 300);
					} 
				},

				unit_tests::testing::test{
					"Test set() [1]",
					[]()
					{
						buffer_2d<uint32_t> buffer{200, 300};
						buffer.set(0, 1, 45);
						assert::is_true(buffer.raw_buffer()[1] == 45);
					}
				},

				unit_tests::testing::test{
					"Test set() [2]",
					[]()
					{
						buffer_2d<uint32_t> buffer{200, 300};
						buffer.set(1, 1, 45);
						assert::is_true(buffer.raw_buffer()[200 + 1] == 45);
					}
				}
			);
		}
	};

	static_assert(testing::has_tests<renderer_tests>);
}
