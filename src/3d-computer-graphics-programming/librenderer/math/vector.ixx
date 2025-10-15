export module librenderer:vector;
import std;

export namespace math
{
	template<typename T>
	concept vector1_like = requires(T v) { { v.x } -> std::convertible_to<float>; };
	template<typename T>
	concept vector2_like = requires(T v) 
	{ 
		{ v.x } -> std::convertible_to<float>;
		{ v.y } -> std::convertible_to<float>;
	};
	template<typename T>
	concept vector3_like = requires(T v)
	{
		{ v.x } -> std::convertible_to<float>;
		{ v.y } -> std::convertible_to<float>;
		{ v.z } -> std::convertible_to<float>;
	};
	template<typename T>
	concept vector4_like = requires(T v)
	{
		{ v.x } -> std::convertible_to<float>;
		{ v.y } -> std::convertible_to<float>;
		{ v.z } -> std::convertible_to<float>;
		{ v.w } -> std::convertible_to<float>;
	};

	template<typename T>
	concept vector_like = 
		vector1_like<T> or vector2_like<T> or vector3_like<T> or vector4_like<T>;

	template<typename T, typename U>
	concept dot_product_defined =
		(vector1_like<T> and vector1_like<U>)
		or (vector2_like<T> and vector2_like<U>)
		or (vector3_like<T> and vector3_like<U>)
		or (vector4_like<T> and vector4_like<U>);

	template<vector_like T, vector_like V>
	auto dot_product(const T& a, const V& b) noexcept -> float
	{
		static_assert(dot_product_defined<decltype(a), decltype(b)>, "a and b must be vectors of matching dimension.");

		float result = 0;
		if constexpr (vector1_like<T>) 
			result += a.x * b.x;
		if constexpr (vector2_like<T>)
			result += a.y * b.y;
		if constexpr (vector3_like<T>)
			result += a.z * b.z;
		if constexpr (vector4_like<T>)
			result += a.w * b.w;
		return result;
	}

	struct vector_skills
	{
		constexpr auto dot_product(this auto&& self, auto&& other) noexcept -> float
		{
			return math::dot_product(self, other);
		}

		constexpr auto magnitude(this vector_like auto&& self) noexcept -> float
		{
			if constexpr (vector1_like<decltype(self)>)
				return std::sqrt(self.x * self.x);
			if constexpr (vector2_like<decltype(self)>)
				return std::sqrt(self.x * self.x + self.y * self.y);
			if constexpr (vector3_like<decltype(self)>)
				return std::sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
			if constexpr (vector4_like<decltype(self)>)
				return std::sqrt(self.x * self.x + self.y * self.y + self.z * self.z + self.w * self.w);
		}

		constexpr void normalise(this auto&& self) noexcept
		{
			float multiplicand = 1.f / self.magnitude();
			if constexpr (requires { self.x; })
				self.x *= multiplicand;
			if constexpr (requires { self.y; })
				self.y *= multiplicand;
			if constexpr (requires { self.z; })
				self.z *= multiplicand;
			if constexpr (requires { self.w; })
				self.w *= multiplicand;
		}

		auto to_normalised(this auto self) noexcept -> auto
		{
			return (self.normalise(), self);
		}

		auto to_rotated_z(this const auto& self, float angle) noexcept -> auto
		{
			decltype(auto) result = self; // preserve type
			result.x = self.x * std::cos(angle) - self.y * std::sin(angle);
			result.y = self.x * std::sin(angle) + self.y * std::cos(angle);
			if constexpr (requires { result.z; })
				result.z = 1; // no change
			return result;
		}
	};

	struct vector_2f : vector_skills
	{
		float x = 0;
		float y = 0;

		constexpr vector_2f() = default;
		constexpr vector_2f(float x, float y) : x(x), y(y) {}
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

	struct vector_4f
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1; // homogenous coordinate
		auto to_vector_3f(this const vector_4f& self) noexcept -> vector_3f
		{
			return { self.x, self.y, self.z };
		}
	};
}
