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
			renderer::vector_3f vec{ .x=1, .y = 9, .z = 2 };
			renderer::normalise(vec);
			Assert::IsTrue(renderer::magnitude(vec) == 1.f);
		}

		TEST_METHOD(TestVectorAddition)
		{
			renderer::vector_3f vec_a{ .x=1, .y=0, .z=2 };
			renderer::vector_3f vec_b{ .x=0, .y=9, .z=2 };
			vec_a = renderer::add(vec_a, vec_b);
			Assert::IsTrue(vec_a.x == 1.f);
			Assert::IsTrue(vec_a.y == 9.f);
			Assert::IsTrue(vec_a.z == 4.f);
		}

		TEST_METHOD(TestVectorSubtraction)
		{
			renderer::vector_3f vec_a{ .x=1, .y=0, .z = 2 };
			renderer::vector_3f vec_b{ .x = 0, .y = 9, .z=2 };
			vec_a = renderer::subtract(vec_a, vec_b);
			Assert::IsTrue(vec_a.x == 1.f);
			Assert::IsTrue(vec_a.y == -9.f);
			Assert::IsTrue(vec_a.z == 0.f);
		}

		TEST_METHOD(TestVectorDotProductUnitParallel)
		{
			renderer::vector_3f vec_a{ .x=1, .y=0, .z=0 };
			renderer::vector_3f vec_b{ .x=1, .y=0, .z = 0 };
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot == 1.f);
		}

		TEST_METHOD(TestVectorDotProductUnitAntiParallel)
		{
			renderer::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			renderer::vector_3f vec_b{ .x = -1, .y = 0, .z = 0};
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot == -1.f);
		}

		TEST_METHOD(TestVectorDotProductUnitOrthogonal)
		{
			renderer::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			renderer::vector_3f vec_b{ .x = 0, .y = 1, .z = 0};
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot == 0.f);
		}

		TEST_METHOD(TestVectorDotProductParallel)
		{
			renderer::vector_3f vec_a{ .x = 5, .y = 0, .z = 0};
			renderer::vector_3f vec_b{ .x = 5, .y = 0, .z = 0};
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot > 1.f);
		}

		TEST_METHOD(TestVectorDotProductAntiParallel)
		{
			renderer::vector_3f vec_a{ .x = 5, .y = 0, .z = 0};
			renderer::vector_3f vec_b{ .x = -5, .y = 0, .z = 0};
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot < -1.f);
		}

		TEST_METHOD(TestVectorDotProductOrthogonal)
		{
			renderer::vector_3f vec_a{ .x = 1, .y = 0, .z = 0};
			renderer::vector_3f vec_b{.x=0, .y = 1, .z=0};
			float dot = renderer::dot_product(vec_a, vec_b);
			Assert::IsTrue(dot == 0.f);
		}

		TEST_METHOD(TestRadiansToDegrees)
		{
			renderer::radians value{std::numbers::pi};
			auto converted = renderer::degrees(value).value;
			Assert::IsTrue(converted == 180.f);
		}

		TEST_METHOD(TestDegreesToRadians)
		{
			renderer::degrees value{180};
			auto converted = renderer::radians(value).value;
			Assert::IsTrue(converted == 3.14159274f);
		}
	};
}
