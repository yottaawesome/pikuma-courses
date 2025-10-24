export module librenderer:shading;
import std;
import :math;

export namespace renderer
{
	struct light
	{
		constexpr light() = default;
		constexpr light(math::vector_4f direction, std::uint32_t color)
			: direction(direction), color(color)
		{ 
			normalise();
		}

		math::vector_4f direction{};
		std::uint32_t color = 0xFFFFFFFF;

		constexpr void normalise(this light& self)
		{
			self.direction.normalise();
		}

		constexpr auto dot(this const light& self, const math::vector_4f& other) -> float
		{
			return self.direction.dot_product(other);
		}

		constexpr auto cross(this const light& self, const math::vector_4f& other) -> math::vector_4f
		{
			return math::cross_product(self.direction, other);
		}

		constexpr auto intensity_from_normal(this const light& self, const math::vector_4f& face_normal) -> float
		{
			auto dot = self.dot(face_normal);
			if (dot >= 0)
				return 0;
			return math::abs(dot);
		}

		constexpr auto apply_light_intensity(this const light& self, float percentage_factor) noexcept -> std::uint32_t
		{
			auto a = (self.color & 0xFF000000);
			auto r = static_cast<std::uint32_t>((self.color & 0x00FF0000) * percentage_factor);
			auto g = static_cast<std::uint32_t>((self.color & 0x0000FF00) * percentage_factor);
			auto b = static_cast<std::uint32_t>((self.color & 0x000000FF) * percentage_factor);

			return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
		}

		constexpr auto compute_intensity_from_normal(this const light& self, const math::vector_4f& face_normal) -> float
		{
			auto intensity = self.intensity_from_normal(face_normal);
			return self.apply_light_intensity(intensity);
		}
	};
}
