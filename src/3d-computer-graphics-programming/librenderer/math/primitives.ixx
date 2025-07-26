export module librenderer:primitives;
import std;

export namespace math
{
	struct point_2d final
	{
		std::uint32_t x = 0;
		std::uint32_t y = 0;
	};

	struct basic_rectangle final
	{
		std::uint32_t width = 0;
		std::uint32_t height = 0;
	};

	struct rectangle final
	{
		point_2d origin{};
		std::uint32_t width = 0;
		std::uint32_t height = 0;
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

		// ---------------------------------
		// Vector dot product -- takes two vectors a and b and yields a scalar.
		// Component formula: f(a, b) = a.x * b.x + a.y * b.y + a.z * b.z.
		// Angle formula: f(a, b) = a · b = |a| × |b| × cos(θ)
		// ---------------------------------
		// f(a, b) < 0 where θ > 90
		// f(a, b) = 0 where θ = 90
		// f(a, b) > 0 where θ < 90
		// Assuming unit vectors a, b, then:
		// f(a, b) = 1 where where θ = 0
		// f(a, b) = 0 where where θ = 90
		// f(a, b) = -1 where where θ = 180
		// ---------------------------------
		static float dot_product(vector_3f a, vector_3f b) noexcept
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		// P x Q = <PyQz - PzQy, PzQx - PxQz, PxQy - PyQx>
		// |P x Q|^2 = |P|^2 × |Q|^2 × sin(θ)^2
		// Anticommutative, not associative.
		// The generated vector follows the right hand rule,
		// if the right hand's fingers are aligned with P 
		// and the palm is facing Q, then the thumb is aligned
		// with the result. For example, taking the cross-product
		// of the x-axis vector (1,0,0) and the y-axis vector 
		// (0,1,0) yields z-vector (0,0,1) with the positive 
		// axis moving toward the viewer (DirectX uses the LHS).
		static vector_3f cross_product(vector_3f a, vector_3f b) noexcept
		{
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

		static vector_3f scale(vector_3f a, float scale) noexcept
		{
			return { a.x * scale, a.y * scale, a.z * scale };
		}
		
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

		vector_3f rotate_x(const float angle) const noexcept
		{
			return {
				x,
				y * std::cos(angle) - z * std::sin(angle),
				y * std::sin(angle) + z * std::cos(angle)
			};
		}

		vector_3f rotate_y(const float angle) const noexcept
		{
			return {
				x * std::cos(angle) - z * std::sin(angle),
				y,
				x * std::sin(angle) + z * std::cos(angle)
			};
		}

		vector_3f rotate_z(const float angle) const noexcept
		{
			return {
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle),
				z
			};
		}

		vector_3f add(vector_3f other) const noexcept // commutative
		{
			return { x + other.x, y + other.y, z + other.z };
		}

		vector_3f subtract(vector_3f other) const noexcept
		{
			return { x - other.x, y - other.y, z - other.z };
		}

		float dot_product(vector_3f other) const noexcept
		{
			return dot_product(*this, other);
		}

		vector_3f cross_product(vector_3f other) const noexcept
		{
			return cross_product(*this, other);
		}

		vector_3f operator+(vector_3f other) const noexcept
		{
			return add(other);
		}

		vector_3f operator+=(vector_3f other) noexcept
		{
			*this = add(other);
			return *this;
		}

		vector_3f operator-(vector_3f other) const noexcept
		{
			return subtract(other);
		}

		vector_3f operator-=(vector_3f other) noexcept
		{
			*this = subtract(other);
			return *this;
		}

		vector_3f operator*(vector_3f other) const noexcept
		{
			return cross_product(*this, other);
		}

		vector_3f operator*=(vector_3f other) noexcept
		{
			*this = cross_product(*this, other);
			return *this;
		}
	};

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
		std::uint32_t color = 0xffadd8e6;
	};

	struct triangle
	{
		vector_2f points[3];
		std::uint32_t color = 0xffffffff;
	};
}
