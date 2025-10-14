export module librenderer:vector;
import std;

export namespace math
{
	struct vector_2f
	{
		float x = 0;
		float y = 0;

		auto magnitude(this const vector_2f& self) noexcept -> float
		{
			return std::sqrt(self.x * self.x + self.y * self.y);
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
			return std::sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
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
}
