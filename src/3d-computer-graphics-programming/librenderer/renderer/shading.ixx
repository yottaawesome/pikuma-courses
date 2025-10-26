export module librenderer:shading;
import std;
import :math;
import :util;

export namespace renderer
{
	struct light
	{
		constexpr light() = default;
		constexpr light(math::vector_4f direction, std::uint32_t color) noexcept
			: direction(direction), color(color)
		{ 
			normalise();
		}

		math::vector_4f direction{};
		std::uint32_t color = 0xFFFFFFFF;

		constexpr void normalise(this light& self) noexcept
		{
			self.direction.normalise();
		}

		constexpr auto dot(this const light& self, const math::vector_4f& other) 
			noexcept -> float
		{
			/*return self.direction.x * other.x
				+ self.direction.y * other.y
				+ self.direction.z * other.z;*/
			return self.direction.dot_product(other);
		}

		constexpr auto cross(this const light& self, const math::vector_4f& other) 
			noexcept -> math::vector_4f
		{
			return math::cross_product(self.direction, other);
		}

		constexpr auto intensity_from_normal(this const light& self, const math::vector_4f& face_normal) 
			noexcept -> float
		{
			auto dot = self.direction.dot_product(face_normal);
			// vector a: ---->
			// vector b: ---->
			// vector a: ---->
			// vector b: <----
			auto percentage_factor = std::clamp(math::abs(dot), 0.f, 1.f);
			if not consteval
			{
				//util::print_debug_string("z {}", face_normal.z);
			}

			return percentage_factor;
		}

		constexpr auto apply_light_intensity(this const light& self, std::uint32_t color, float percentage_factor) 
			noexcept -> std::uint32_t
		{
			auto a = (color & 0xFF000000);
			auto r = static_cast<std::uint32_t>((color & 0x00FF0000) * percentage_factor);
			auto g = static_cast<std::uint32_t>((color & 0x0000FF00) * percentage_factor);
			auto b = static_cast<std::uint32_t>((color & 0x000000FF) * percentage_factor);

			return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
		}

		constexpr auto compute_intensity_from_normal(this const light& self, const math::vector_4f& face_normal) 
			noexcept -> std::uint32_t
		{
			auto intensity = self.intensity_from_normal(face_normal);
			return self.apply_light_intensity(self.color, intensity);
		}

		constexpr auto compute_intensity_from_normal(this const light& self, std::uint32_t color, const math::vector_4f& face_normal)
			noexcept -> std::uint32_t
		{
			auto intensity = self.intensity_from_normal(face_normal);
			return self.apply_light_intensity(color, intensity);
		}
	};
}
