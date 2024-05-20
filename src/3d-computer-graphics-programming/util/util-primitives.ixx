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

		float magnitude() const noexcept 
		{ 
			return std::sqrt(x*x+y*y); 
		}

		void normalise() noexcept
		{
			float multiplicand = 1.f / magnitude();
			x *= multiplicand;
			y *= multiplicand;
		}

		vector_2f to_normalised() const noexcept
		{
			vector_2f v{ *this };
			v.normalise();
			return v;
		}

		vector_2f to_rotated_z(const float angle) const noexcept
		{
			return {
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
			};
		}
	};

	struct vector_3f
	{
		float x = 0;
		float y = 0;
		float z = 0;
		
		float magnitude() const noexcept 
		{ 
			return std::sqrt(x*x+y*y+z*z); 
		}
		
		void normalise() noexcept
		{
			const float multiplicand = 1.f / magnitude();
			x *= multiplicand;
			y *= multiplicand;
			z *= multiplicand;
		}

		vector_3f to_normalised() const noexcept
		{
			vector_3f v{ *this };
			v.normalise();
			return v;
		}

		vector_3f to_rotated_x(const float angle) const noexcept
		{
			return {
				x,
				y * std::cos(angle) - z * std::sin(angle),
				y * std::sin(angle) + z * std::cos(angle)
			};
		}

		vector_3f to_rotated_y(const float angle) const noexcept
		{
			return {
				x * std::cos(angle) - z * std::sin(angle),
				y,
				x * std::sin(angle) + z * std::cos(angle)
			};
		}

		vector_3f to_rotated_z(const float angle) const noexcept
		{
			return {
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle),
				z
			};
		}
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

	bool operator==(const degrees&, const radians&) = delete;
	
	radians convert(degrees v) noexcept
	{
		return { v.value * static_cast<float>(std::numbers::pi) / 180.f };
	}

	degrees convert(radians v) noexcept
	{
		return { v.value * 180.f / static_cast<float>(std::numbers::pi) };
	}

	struct face
	{
		int a = 0;
		int b = 0;
		int c = 0;
	};

	struct triangle
	{
		util::vector_2f points[3];
	};
}