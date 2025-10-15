#include "pch.h"
#include "CppUnitTest.h"
import librenderer;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PrimitivesTests)
	{
		TEST_METHOD(TestNormalisation)
		{
			math::vector_3f vec{ .x=1, .y = 9, .z = 2 };
			vec.normalise();
			Assert::IsTrue(vec.magnitude() == 1.f);
		}

		TEST_METHOD(TestVectorAddition)
		{
			math::vector_3f vec_a{ .x=1, .y=0, .z=2 };
			math::vector_3f vec_b{ .x=0, .y=9, .z=2 };
			vec_a = vec_a.add(vec_b);
			Assert::IsTrue(vec_a.x == 1.f);
			Assert::IsTrue(vec_a.y == 9.f);
			Assert::IsTrue(vec_a.z == 4.f);
		}

		TEST_METHOD(TestVectorSubtraction)
		{
			math::vector_3f vec_a{ .x=1, .y=0, .z = 2 };
			math::vector_3f vec_b{ .x = 0, .y = 9, .z=2 };
			vec_a = vec_a.subtract(vec_b);
			Assert::IsTrue(vec_a.x == 1.f);
			Assert::IsTrue(vec_a.y == -9.f);
			Assert::IsTrue(vec_a.z == 0.f);
		}

		TEST_METHOD(TestVectorDotProductUnitParallel)
		{
			math::vector_3f vec_a{ .x=1, .y=0, .z=0 };
			math::vector_3f vec_b{ .x=1, .y=0, .z = 0 };
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot == 1.f);
		}

		TEST_METHOD(TestVectorDotProductUnitAntiParallel)
		{
			math::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			math::vector_3f vec_b{ .x = -1, .y = 0, .z = 0};
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot == -1.f);
		}

		TEST_METHOD(TestVectorDotProductUnitOrthogonal)
		{
			math::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			math::vector_3f vec_b{ .x = 0, .y = 1, .z = 0};
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot == 0.f);
		}

		TEST_METHOD(TestVectorDotProductParallel)
		{
			math::vector_3f vec_a{ .x = 5, .y = 0, .z = 0};
			math::vector_3f vec_b{ .x = 5, .y = 0, .z = 0};
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot > 1.f);
		}

		TEST_METHOD(TestVectorDotProductAntiParallel)
		{
			math::vector_3f vec_a{ .x = 5, .y = 0, .z = 0};
			math::vector_3f vec_b{ .x = -5, .y = 0, .z = 0};
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot < -1.f);
		}

		TEST_METHOD(TestVectorDotProductOrthogonal)
		{
			math::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			math::vector_3f vec_b{.x=0, .y = 1, .z=0};
			float dot = vec_a.dot_product(vec_b);
			Assert::IsTrue(dot == 0.f);
		}

		TEST_METHOD(TestRadiansToDegrees)
		{
			math::radians value{std::numbers::pi};
			auto converted = math::convert(value);
			Assert::IsTrue(converted == 180.f);
		}

		TEST_METHOD(TestDegreesToRadians)
		{
			math::degrees value{180};
			auto converted = math::convert(value);
			Assert::IsTrue(converted == 3.14159274f);
		}
	};
}
