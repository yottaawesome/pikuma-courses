export module librenderer:math.vector;
import std;
import :math.functions;

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
	constexpr auto dot_product(const T& a, const V& b) noexcept -> float
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
			return math::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if constexpr (vector3_like<decltype(v)>)
			return math::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if constexpr (vector2_like<decltype(v)>)
			return math::sqrt(v.x * v.x + v.y * v.y);
		/*if constexpr (vector1_like<decltype(v)>)
			return std::sqrt(v.x * v.x);*/
	}

	constexpr void normalise(vector_like auto& v) noexcept
	{
		float multiplicand = 1.f / math::magnitude(v);
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

	// Forward declaration of vector_4f for conversion operator.
	struct vector_4f;

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
		constexpr operator vector_4f(this const vector_3f& self) noexcept;
	};

	struct vector_4f
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

	constexpr auto to_normalised(vector_like auto v) noexcept
	{
		return (math::normalise(v), v);
	}

	constexpr auto operator+(const vector_like auto& lhs, const vector_like auto& rhs)
		noexcept -> std::remove_cvref_t<decltype(lhs)>
	{
		return math::add(lhs, rhs);
	}

	constexpr auto operator+=(vector_like auto& lhs, const vector_like auto& rhs)
		noexcept -> decltype(lhs)&
	{
		return (lhs = math::add(lhs, rhs));
	}

	constexpr auto operator-(const vector_like auto& lhs, const vector_like auto& rhs)
		noexcept -> std::remove_cvref_t<decltype(lhs)>
	{
		return math::subtract(lhs, rhs);
	}

	constexpr auto operator-=(vector_like auto& lhs, const vector_like auto& rhs)
		noexcept -> decltype(lhs)&
	{
		return (lhs = math::subtract(lhs, rhs));
	}

	constexpr auto operator==(const vector_like auto& lhs, const vector_like auto& rhs)
		noexcept -> bool
	{
		if constexpr (vector3_like<decltype(lhs)> or vector4_like<decltype(lhs)>)
			return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z;
		if constexpr (vector2_like<decltype(lhs)>)
			return lhs.x == rhs.x and lhs.y == rhs.y;
		if constexpr (vector1_like<decltype(lhs)>)
			return lhs.x == rhs.x;
	}

	constexpr auto rotate_x(const vector3_like auto& v, float angle)
		noexcept -> std::remove_cvref_t<decltype(v)>
	{
		return {
			.x = v.x,
			.y = v.y * std::cos(angle) - v.z * std::sin(angle),
			.z = v.y * std::sin(angle) + v.z * std::cos(angle)
		};
	}

	constexpr void rotate_x_in_place(vector3_like auto& v, float angle) noexcept
	{
		auto newY = v.y * std::cos(angle) - v.z * std::sin(angle);
		auto newZ = v.y * std::sin(angle) + v.z * std::cos(angle);
		v.y = newY;
		v.z = newZ;
	}

	constexpr auto rotate_y(const vector3_like auto& v, float angle)
		noexcept -> std::remove_cvref_t<decltype(v)>
	{
		return {
			.x = v.x * std::cos(angle) - v.z * std::sin(angle),
			.y = v.y,
			.z = v.x * std::sin(angle) + v.z * std::cos(angle)
		};
	}

	constexpr void rotate_y_in_place(vector3_like auto& v, float angle) noexcept
	{
		auto newX = v.x * std::cos(angle) - v.z * std::sin(angle);
		auto newZ = v.x * std::sin(angle) + v.z * std::cos(angle);
		v.x = newX;
		v.z = newZ;
	}

	constexpr auto rotate_z(const vector3_like auto& v, float angle)
		noexcept -> std::remove_cvref_t<decltype(v)>
	{
		return {
			.x = v.x * std::cos(angle) - v.y * std::sin(angle),
			.y = v.x * std::sin(angle) + v.y * std::cos(angle),
			.z = v.z
		};
	}

	constexpr void rotate_z_in_place(vector3_like auto& v, float angle) noexcept
	{
		auto newX = v.x * std::cos(angle) - v.y * std::sin(angle);
		auto newY = v.x * std::sin(angle) + v.y * std::cos(angle);
		v.x = newX;
		v.y = newY;
	}

	constexpr void rotate_xyz_in_place(
		vector3_like auto& v,
		float x_angle,
		float y_angle,
		float z_angle
	) noexcept
	{
		math::rotate_x_in_place(v, x_angle);
		math::rotate_y_in_place(v, y_angle);
		math::rotate_z_in_place(v, z_angle);
	}

	constexpr auto rotate_z_2d(const vector2_like auto& v, float angle)
		noexcept -> std::remove_cvref_t<decltype(v)>
	{
		return {
			.x = v.x * std::cos(angle) - v.y * std::sin(angle),
			.y = v.x * std::sin(angle) + v.y * std::cos(angle)
		};
	}

	/*
	Barycentric coordinates are a set of weights that express the position of a point
	relative to the vertices of a triangle. They can be thought of as weights assigned
	to each vertex. They are represented typically as (α,β,γ), where:
		α = area_triangle(PBC) / area_triangle(ABC)
		β = area_triangle(APC) / area_triangle(ABC)
		γ = area_triangle(ABP) / area_triangle(ABC)

	Note that α + β + γ = 1.

	Due to the cross-product giving us a parallelogram area to the two vectors, we
	can also express the above as the following:
		α = (area_parallelogram(PBEC)/2) / (area_parallelogram(ABCD)/2)
		β = (area_parallelogram(APEC)/2) / (area_parallelogram(ABCD)/2)
	which simplifies to:
		α = area_parallelogram(PBEC) / area_parallelogram(ABCD).
		β = area_parallelogram(APEC) / area_parallelogram(ABCD).

	PBEC is  the subparallelogram formed by points P, B, E and C, and ABCD is the
	parallelogram formed by points A, B, C and D.

	Thus,
		α = ||PC x PB|| / ||AC x AB||
		β = ||AC x AP|| / ||AC x AB||

	With α and β known, we can find γ using the identity above:
		γ = 1 - α - β

	Note that this does not currently incorporate perspective correction. To fix
	the distortion caused by perspective, we need to find the depth at point P
	inside the triangle. Perspective is not linear, so we cannot simply linearly
	interpolate the depths of the triangle vertices using barycentric coordinates;
	z is no linear across the triangle. But the reciprocal of z (1/z) is linear and
	so we can use 1/z to find the interpolated z/depth at point P.
	*/
	constexpr auto barycentric_weights(
		const vector2_like auto& a,
		const vector2_like auto& b,
		const vector2_like auto& c,
		const vector2_like auto& p
	) noexcept -> vector_3f
	{
		auto ac = c - a;
		auto ab = b - a;
		auto pc = c - p;
		auto pb = b - p;
		auto ap = p - a;

		// Pseudo-cross products to get areas.
		auto area_parallelologram_abc = ac.x * ab.y - ac.y * ab.x;

		float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelologram_abc;
		float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelologram_abc;
		float gamma = 1.f - alpha - beta;
		// Don't clamp individual weights -- doing so breaks the
		// alpha + beta + gamma = 1 invariant, which corrupts
		// interpolated 1/w values and causes depth test artifacts.
		return { .x = alpha, .y = beta, .z = gamma };
	}
}
