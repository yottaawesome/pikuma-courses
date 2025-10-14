export module librenderer:vector;
import std;

export namespace math
{
	template<typename T>
	concept x_component = requires(T t){ t.x; };
	template<typename T, typename V>
	concept x_components = x_component<T> and x_component<V>;
	template<typename T, typename V>
	concept one_has_x_component = x_component<T> or x_component<V>;
	template<typename T, typename V>
	concept incompatible_x = not x_components<T, V> and one_has_x_component<T, V>;

	template<typename T>
	concept y_component = requires(T t) { t.y; };
	template<typename T, typename V>
	concept y_components = y_component<T> and y_component<V>;
	template<typename T, typename V>
	concept one_has_y_component = y_component<T> or y_component<V>;
	template<typename T, typename V>
	concept incompatible_y = not y_components<T, V> and one_has_y_component<T, V>;

	template<typename T>
	concept z_component = requires(T t) { t.z; };
	template<typename T, typename V>
	concept z_components = z_component<T> and z_component<V>;
	template<typename T, typename V>
	concept one_has_z_component = z_component<T> or z_component<V>;
	template<typename T, typename V>
	concept incompatible_z = not z_components<T, V> and one_has_z_component<T, V>;

	template<typename T>
	concept w_component = requires(T t) { t.w; };
	template<typename T, typename V>
	concept w_components = w_component<T> and w_component<V>;
	template<typename T, typename V>
	concept one_has_w_component = w_component<T> or w_component<V>;
	template<typename T, typename V>
	concept incompatible_w = not w_components<T, V> and one_has_w_component<T, V>;

	template<typename T>
	concept vector_like = 
		x_component<T> 
		or y_component<T> 
		or z_component<T> 
		or w_component<T>;

	template<typename U, typename V>
	concept matching_vector_dimensions = 
		[]
		{
			U u{}; V v{};
			if constexpr (incompatible_x<U, V>)
				return false;
			if constexpr (incompatible_y<U, V>)
				return false;
			if constexpr (incompatible_z<U, V>)
				return false;
			if constexpr (incompatible_w<U, V>)
				return false;
			return vector_like<U> and vector_like<V>;
		}();

	template<vector_like T, vector_like V>
	auto dot_product(const T& a, const V& b) noexcept -> float
	{
		static_assert(matching_vector_dimensions<decltype(a), decltype(b)>, "a and b must have matching vector components");

		float x = 0, y = 0, z = 0, w = 0;
		if constexpr (x_components<T, V>) 
			x = a.x * b.x;
		if constexpr (y_components<T, V>)
			x = a.y * b.y;
		if constexpr (z_components<T, V>)
			x = a.z * b.z;
		if constexpr (w_components<T, V>)
			w = a.w * b.w;
		return x + y + z + w;
	}

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
