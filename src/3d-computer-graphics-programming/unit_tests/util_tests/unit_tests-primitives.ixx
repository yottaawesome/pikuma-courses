export module unit_tests:primitives;
import std;
import util;
import :framework;

export namespace unit_tests::util_tests
{
	struct primitive_tests
	{
		auto tests() -> testing::testables auto
		{
			return unit_tests::testing::make_tests(
				unit_tests::testing::test{
					"Test normalisation",
					[] {
						util::vector_3f vec{1, 9, 2};
						vec.normalise();
						unit_tests::assert::is_true(vec.magnitude() == 1.f);
					}
				},

				unit_tests::testing::test{
					"Test vector addition",
					[] {
						util::vector_3f vec_a{1, 0, 2};
						util::vector_3f vec_b{0, 9, 2};
						vec_a = vec_a.add(vec_b);
						unit_tests::assert::is_true(vec_a.x == 1.f);
						unit_tests::assert::is_true(vec_a.y == 9.f);
						unit_tests::assert::is_true(vec_a.z == 4.f);
					}
				},

				unit_tests::testing::test{
					"Test vector subtraction",
					[] {
						util::vector_3f vec_a{1, 0, 2};
						util::vector_3f vec_b{0, 9, 2};
						vec_a = vec_a.subtract(vec_b);
						unit_tests::assert::is_true(vec_a.x == 1.f);
						unit_tests::assert::is_true(vec_a.y == -9.f);
						unit_tests::assert::is_true(vec_a.z == 0.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (unit parallel)",
					[] {
						util::vector_3f vec_a{1, 0, 0};
						util::vector_3f vec_b{1, 0, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot == 1.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (unit anti-parallel)",
					[] {
						util::vector_3f vec_a{1, 0, 0};
						util::vector_3f vec_b{-1, 0, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot == -1.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (unit orthogonal)",
					[] {
						util::vector_3f vec_a{1, 0, 0};
						util::vector_3f vec_b{0, 1, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot == 0.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (parallel)",
					[] {
						util::vector_3f vec_a{5, 0, 0};
						util::vector_3f vec_b{5, 0, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot > 1.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (anti-parallel)",
					[] {
						util::vector_3f vec_a{5, 0, 0};
						util::vector_3f vec_b{-5, 0, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot < -1.f);
					}
				},

				unit_tests::testing::test{
					"Test vector dot product (orthogonal)",
					[] {
						util::vector_3f vec_a{1, 0, 0};
						util::vector_3f vec_b{0, 1, 0};
						float dot = vec_a.dot_product(vec_b);
						unit_tests::assert::is_true(dot == 0.f);
					}
				}
			);
		}
	};

	static_assert(testing::test_exporter<primitive_tests>);
}
