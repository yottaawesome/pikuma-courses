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
		auto is_in_point_space(point_2d point) const noexcept -> bool
		{
			return (origin.x + width < point.x) and (origin.y + height < point.y);
		}
	};

	struct vector_2f
	{
		float x = 0;
		float y = 0;

		auto magnitude(this const vector_2f& self) noexcept -> float
		{ 
			return std::sqrt(self.x*self.x + self.y*self.y);
		}

		void normalise(this vector_2f& self) noexcept
		{
			float multiplicand = 1.f / self.magnitude();
			self.x *= multiplicand;
			self.y *= multiplicand;
		}

		auto to_normalised(this vector_2f self) noexcept -> vector_2f
		{
			return (self.normalise(), self);
		}

		auto to_rotated_z(this const vector_2f& self, float angle) noexcept -> vector_2f
		{
			return {
				self.x * std::cos(angle) - self.y * std::sin(angle),
				self.x * std::sin(angle) + self.y * std::cos(angle)
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
		static auto dot_product(vector_3f a, vector_3f b) noexcept -> float
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
		static auto cross_product(vector_3f a, vector_3f b) noexcept -> vector_3f
		{
			return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
		}

		static auto scale(vector_3f a, float scale) noexcept -> vector_3f
		{
			return { a.x * scale, a.y * scale, a.z * scale };
		}
		
		auto magnitude(this const vector_3f& self) noexcept -> float
		{ 
			return std::sqrt(self.x*self.x + self.y*self.y + self.z*self.z);
		}
		
		void normalise(this vector_3f& self) noexcept
		{
			float multiplicand = 1.f / self.magnitude();
			self.x *= multiplicand;
			self.y *= multiplicand;
			self.z *= multiplicand;
		}

		auto to_normalised(this vector_3f v) noexcept -> vector_3f
		{
			return (v.normalise(), v);
		}

		auto rotate_x(this const vector_3f& self, float angle) noexcept -> vector_3f
		{
			return {
				self.x,
				self.y * std::cos(angle) - self.z * std::sin(angle),
				self.y * std::sin(angle) + self.z * std::cos(angle)
			};
		}

		auto rotate_y(this const vector_3f& self, float angle) noexcept -> vector_3f
		{
			return {
				self.x * std::cos(angle) - self.z * std::sin(angle),
				self.y,
				self.x * std::sin(angle) + self.z * std::cos(angle)
			};
		}

		auto rotate_z(this const vector_3f& self, float angle) noexcept -> vector_3f
		{
			return {
				self.x * std::cos(angle) - self.y * std::sin(angle),
				self.x * std::sin(angle) + self.y * std::cos(angle),
				self.z
			};
		}

		auto add(this const vector_3f& self, vector_3f other) noexcept -> vector_3f // commutative
		{
			return { self.x + other.x, self.y + other.y, self.z + other.z };
		}

		auto subtract(this const vector_3f& self, vector_3f other) noexcept -> vector_3f
		{
			return { self.x - other.x, self.y - other.y, self.z - other.z };
		}

		auto dot_product(this const vector_3f& self, vector_3f other) noexcept -> float
		{
			return self.dot_product(self, other);
		}

		auto cross_product(this const vector_3f& self, vector_3f other) noexcept -> vector_3f
		{
			return self.cross_product(self, other);
		}

		auto operator+(this const vector_3f& self, vector_3f other) noexcept -> vector_3f
		{
			return self.add(other);
		}

		auto operator+=(this vector_3f& self, vector_3f other) noexcept -> vector_3f&
		{
			self = self.add(other);
			return self;
		}

		auto operator-(this const vector_3f& self, vector_3f other) noexcept -> vector_3f
		{
			return self.subtract(other);
		}

		auto operator-=(this vector_3f& self, vector_3f other) noexcept -> vector_3f&
		{
			return (self = self.subtract(other), self);
		}

		auto operator*(this const vector_3f& self, vector_3f other) noexcept -> vector_3f
		{
			return self.cross_product(self, other);
		}

		auto operator*=(this vector_3f& self, vector_3f other) noexcept -> vector_3f&
		{
			return (self = self.cross_product(self, other), self);
		}
	};

	struct radians final 
	{ 
		float value = 0; 
		operator float() const noexcept { return value; }
		auto operator<=>(const radians&) const = default;
		auto operator==(float rhs) const noexcept -> bool { return value == rhs; }
	};

	struct degrees final 
	{ 
		float value = 0;
		operator float() const noexcept { return value; }
		auto operator<=>(const degrees&) const = default;
		auto operator==(float rhs) const noexcept -> bool { return value == rhs; }
	};

	bool operator==(const degrees&, const radians&) = delete;
	
	auto convert(degrees v) noexcept -> radians
	{
		return { v.value * static_cast<float>(std::numbers::pi) / 180.f };
	}

	auto convert(radians v) noexcept -> degrees
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
		float average_depth = 0; // for painter's algorithm
	};
}
