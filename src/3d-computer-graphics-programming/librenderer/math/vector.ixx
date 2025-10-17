export module librenderer:vector;
import std;

export namespace math
{
	template<typename T>
	concept vector1_like = requires(T v) { { v.x } -> std::convertible_to<float>; };
	template<typename T>
	concept vector2_like = 
		vector1_like<T> and requires(T v) { { v.y } -> std::convertible_to<float>; };
	template<typename T>
	concept vector3_like = 
		vector2_like<T> and requires(T v) { { v.z } -> std::convertible_to<float>; };
	template<typename T>
	concept vector4_like = 
		vector3_like<T> and requires(T v) { { v.w } -> std::convertible_to<float>; };

	template<typename T>
	concept vector_like = 
		vector1_like<T> or vector2_like<T> or vector3_like<T> or vector4_like<T>;

	template<typename T, typename U>
	concept dot_product_defined =
		(vector1_like<T> and vector1_like<U>)
		or (vector2_like<T> and vector2_like<U>)
		or (vector3_like<T> and vector3_like<U>)
		or (vector4_like<T> and vector4_like<U>);

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
	template<vector_like T, vector_like V>
	auto dot_product(const T& a, const V& b) noexcept -> float
	{
		static_assert(dot_product_defined<decltype(a), decltype(b)>, "a and b must be vectors of matching dimension.");

		if constexpr (vector3_like<T> or vector4_like<T>)
			return a.x * b.x + a.y * b.y + a.z * b.z;
		if constexpr (vector2_like<T>)
			return a.x * b.x + a.y * b.y;
		if constexpr (vector1_like<T>) 
			return a.x * b.x;
	}

	constexpr auto magnitude(vector_like auto&& v) noexcept -> float
	{
		if constexpr (vector4_like<decltype(v)>)
			return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
		if constexpr (vector3_like<decltype(v)>)
			return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if constexpr (vector2_like<decltype(v)>)
			return std::sqrt(v.x * v.x + v.y * v.y);
		if constexpr (vector1_like<decltype(v)>)
			return std::sqrt(v.x * v.x);
	}

	constexpr auto normalise(vector_like auto& v) noexcept
	{
		float multiplicand = 1.f / v.magnitude();
		if constexpr (requires { v.x; })
			v.x *= multiplicand;
		if constexpr (requires { v.y; })
			v.y *= multiplicand;
		if constexpr (requires { v.z; })
			v.z *= multiplicand;
		/*if constexpr (requires { v.w; })
			v.w *= multiplicand;*/
	}

	auto add(const vector_like auto& v, const vector_like auto& u) 
		noexcept -> std::remove_cvref_t<decltype(v)> // commutative
	{
		static_assert(dot_product_defined<decltype(v), decltype(u)>, "a and b must be vectors of matching dimension.");

		if constexpr (vector3_like<decltype(v)> or vector4_like<decltype(v)>)
			return { .x = v.x + u.x, .y = v.y + u.y, .z = v.z + u.z };
		if constexpr (vector2_like<decltype(v)>)
			return { .x = v.x + u.x, .y = v.y + u.y };
		if constexpr (vector1_like<decltype(v)>)
			return { .x = v.x + u.x };
	}

	auto subtract(const vector_like auto& v, const vector_like auto& u) 
		noexcept -> std::remove_cvref_t<decltype(v)>
	{
		if constexpr (vector3_like<decltype(v)> or vector4_like<decltype(v)>)
			return { .x = v.x - u.x, .y = v.y - u.y, .z = v.z - u.z };
		if constexpr (vector2_like<decltype(v)>)
			return { .x = v.x - u.x, .y = v.y - u.y };
		if constexpr (vector1_like<decltype(v)>)
			return { .x = v.x - u.x };
	}

	auto scale(const vector_like auto& vector, float scale) 
		noexcept -> std::remove_cvref_t<decltype(vector)>
	{
		if constexpr (vector3_like<decltype(vector)> or vector4_like<decltype(vector)>)
			return { .x = vector.x * scale, .y = vector.y * scale, .z = vector.z * scale };
		if constexpr (vector2_like<decltype(vector)>)
			return { .x = vector.x * scale, .y = vector.y * scale };
		if constexpr (vector1_like<decltype(vector)>)
			return { .x = vector.x * scale };
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
	constexpr auto cross_product(vector3_like auto a, vector3_like auto b) 
		noexcept -> std::remove_cvref_t<decltype(a)>
	{
		return {
			.x = a.y * b.z - a.z * b.y,
			.y = a.z * b.x - a.x * b.z,
			.z = a.x * b.y - a.y * b.x
		};
	}

	struct euclidean_vector
	{
		constexpr auto dot_product(this auto&& self, auto&& other) 
			noexcept -> float
		{
			return math::dot_product(self, other);
		}

		constexpr auto magnitude(this vector_like auto&& self) 
			noexcept -> float
		{
			return math::magnitude(self);
		}

		constexpr void normalise(this auto&& self) noexcept
		{
			math::normalise(self);
		}

		constexpr auto to_normalised(this auto self) noexcept -> auto
		{
			return (math::normalise(self), self);
		}

		constexpr auto add(this const auto& self, const vector_like auto& other) 
			noexcept -> auto
		{
			return math::add(self, other);
		}

		constexpr auto subtract(this const auto& self, const vector_like auto& other) 
			noexcept -> auto
		{
			return math::subtract(self, other);
		}

		constexpr auto operator+(this const auto& self, const vector_like auto& other)
			noexcept -> auto
		{
			return self.add(other);
		}

		constexpr auto operator+=(this auto& self, vector_like auto other) 
			noexcept -> auto&
		{
			self = self.add(other);
			return self;
		}

		constexpr auto operator-(this const auto& self, vector_like auto other) 
			noexcept -> auto
		{
			return self.subtract(other);
		}

		constexpr auto operator==(this const auto& self, vector_like auto other) 
			noexcept -> bool
		{
			if constexpr (vector3_like<decltype(self)> or vector4_like<decltype(self)>)
				return self.x == other.x and self.y == other.y and self.z == other.z;
			if constexpr (vector2_like<decltype(self)>)
				return self.x == other.x and self.y == other.y;
			if constexpr (vector1_like<decltype(self)>)
				return self.x == other.x;
		}
	};

	struct vector_2f : euclidean_vector
	{
		float x = 0;
		float y = 0;
		
		constexpr auto to_rotated_z(this const auto& self, float angle) 
			noexcept -> vector_2f
		{
			return {
				.x = self.x * std::cos(angle) - self.y * std::sin(angle),
				.y = self.x * std::sin(angle) + self.y * std::cos(angle)
			};
		}

		auto operator-=(this auto& self, vector_like auto other) noexcept -> auto&
		{
			return (self = self.subtract(other), self);
		}

		auto operator*(this const auto& self, vector_like auto other) noexcept -> auto
		{
			return self.cross_product(self, other);
		}

		auto operator*=(this auto& self, vector_like auto other) noexcept -> auto&
		{
			return (self = self.cross_product(self, other), self);
		}
	};

	struct three_space_euclidean_vector
	{
		constexpr auto rotate_x(this const auto& self, float angle) 
			noexcept -> std::remove_cvref_t<decltype(self)>
		{
			return {
				.x = self.x,
				.y = self.y * std::cos(angle) - self.z * std::sin(angle),
				.z = self.y * std::sin(angle) + self.z * std::cos(angle)
			};
		}

		constexpr auto rotate_x_in_place(this auto& self, float angle) noexcept
		{
			auto newY = self.y * std::cos(angle) - self.z * std::sin(angle);
			auto newZ = self.y * std::sin(angle) + self.z * std::cos(angle);
			self.y = newY;
			self.z = newZ;
		}

		constexpr auto rotate_y(this const auto& self, float angle) 
			noexcept -> std::remove_cvref_t<decltype(self)>
		{
			return {
				.x = self.x * std::cos(angle) - self.z * std::sin(angle),
				.y = self.y,
				.z = self.x * std::sin(angle) + self.z * std::cos(angle)
			};
		}

		constexpr void rotate_y_in_place(this auto& self, float angle) noexcept
		{
			auto newX = self.x * std::cos(angle) - self.z * std::sin(angle);
			auto newZ = self.x * std::sin(angle) + self.z * std::cos(angle);
			self.x = newX;
			self.z = newZ;
		}

		constexpr auto rotate_z(this const auto& self, float angle) noexcept 
			-> std::remove_cvref_t<decltype(self)>
		{
			return {
				.x = self.x * std::cos(angle) - self.y * std::sin(angle),
				.y = self.x * std::sin(angle) + self.y * std::cos(angle),
				.z = self.z
			};
		}

		constexpr void rotate_z_in_place(this auto& self, float angle) noexcept
		{
			auto newX = self.x * std::cos(angle) - self.y * std::sin(angle);
			auto newY = self.x * std::sin(angle) + self.y * std::cos(angle);
			self.x = newX;
			self.y = newY;
		}

		constexpr auto cross_product(this const auto& self, auto other)
			noexcept -> auto
		{
			return math::cross_product(self, other);
		}
	};

	// Forward declaration of vector_4f for conversion operator.
	struct vector_4f;
	struct vector_3f : euclidean_vector, three_space_euclidean_vector
	{
		float x = 0;
		float y = 0;
		float z = 0;
		constexpr operator vector_4f(this const vector_3f& self) noexcept;
	};

	struct vector_4f : euclidean_vector, three_space_euclidean_vector
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1; // homogenous coordinate
		constexpr operator vector_3f(this const vector_4f& self) noexcept
		{
			return { .x = self.x, .y = self.y, .z = self.z };
		}
	};

	constexpr vector_3f::operator vector_4f(this const vector_3f& self) noexcept
	{
		return { .x = self.x, .y = self.y, .z = self.z };
	}
}
