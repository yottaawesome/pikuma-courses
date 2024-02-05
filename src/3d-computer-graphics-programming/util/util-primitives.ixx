export module util:primitives;
import shared;

export namespace util
{
	struct point_2d final
	{
		uint32_t x = 0;
		uint32_t y = 0;
	};

	struct basic_rectangle final
	{
		uint32_t width = 0;
		uint32_t height = 0;
	};

	struct rectangle final
	{
		point_2d origin{};
		uint32_t width = 0;
		uint32_t height = 0;
		bool is_in_point_space(const point_2d point) const noexcept
		{
			return (origin.x + width < point.x) and (origin.y + height < point.y);
		}
	};

	struct vector_2f
	{
		float x = 0;
		float y = 0;
	};

	struct vector_3f
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	vector_3f rotate_x(vector_3f v, float angle)
	{
		return {
			v.x,
			v.y * std::cos(angle) - v.z * std::sin(angle),
			v.y * std::sin(angle) + v.z * std::cos(angle)
		};
	}

	vector_3f rotate_y(vector_3f v, float angle)
	{
		return {
			v.x * std::cos(angle) - v.z * std::sin(angle),
			v.y,
			v.x * std::sin(angle) + v.z * std::cos(angle)
		};
	}

	vector_3f rotate_z(vector_3f v, float angle)
	{
		return {
			v.x * std::cos(angle) - v.y * std::sin(angle),
			v.x * std::sin(angle) + v.y * std::cos(angle),
			v.z
		};
	}

	struct radians final 
	{ 
		float value = 0; 
		operator float() const noexcept { return value; }
		auto operator<=>(const radians&) const = default;
		bool operator==(float rhs) const noexcept { return value == rhs; }
	};

	struct degrees final 
	{ 
		float value = 0;
		operator float() const noexcept { return value; }
		auto operator<=>(const degrees&) const = default;
		bool operator==(float rhs) const noexcept { return value == rhs; }
	};
	
	radians convert(degrees v) noexcept
	{
		return { v.value * static_cast<float>(std::numbers::pi) / 180.f };
	}

	degrees convert(radians v) noexcept
	{
		return { v.value * 180.f / static_cast<float>(std::numbers::pi) };
	}
}