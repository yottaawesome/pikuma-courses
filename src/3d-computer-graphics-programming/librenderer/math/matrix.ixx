export module librenderer:matrix;
import std;
import :concepts;
import :vector;
import :degreesradians;

export namespace math
{
	// Linear transformation matrices define a space as 
	// an orthogonal basis with respect to the origin.
	// Affine tranformation matrices are not in 
	// reference to an origin.

	// Interesting note: the result of a shear in 3D is
	// a translation in 2D. This actually holds for all
	// dimensions: a shear in N dimensions results in a
	// translation in N-1 dimensions.

	// Row-major and column-major affect the way matrices
	// are multiplied with vectors. In a row-major system,
	// vectors are treated as row vectors and multiplied
	// on the left side of matrices (pre-multiplication). 
	// In a column-major, vectors are treated as column 
	// vectors and multiplied on the right side of 
	// matrices (post-multiplication).

	template<concepts::is_arithmetic TArithmetic, std::uint32_t VRows, std::uint32_t VColumns>
	struct matrix
	{
		static_assert(VRows > 0 and VColumns > 0, "Matrix must have at least one row and one column.");
		constexpr matrix() = default;

		constexpr matrix(std::convertible_to<TArithmetic> auto...values)
			: Values{ static_cast<TArithmetic>(values)... }
		{
			// C++26?
			/*for(int i = 0; i < VRows * VColumns; ++i)
				Values[i / VColumns][i % VColumns] = values[i];*/
		}

		constexpr auto operator[](this auto&& self, std::uint32_t row) 
			noexcept -> decltype(auto)
		{
			return std::forward_like<decltype(self)>(self.Values[row]);
		}

		constexpr auto operator[](
			this auto&& self,
			std::uint32_t row,
			std::uint32_t column
		) noexcept -> decltype(auto)
		{
			return std::forward_like<decltype(self)>(self.Values[row][column]);
		}

		constexpr auto rows() const noexcept -> std::uint32_t 
		{ 
			return VRows; 
		}

		constexpr auto columns() const noexcept -> std::uint32_t 
		{
			return VColumns; 
		}

		constexpr auto operator*(this matrix self, TArithmetic scalar) noexcept -> matrix
		{
			for (std::uint32_t r = 0; r < VRows; ++r)
				for (std::uint32_t c = 0; c < VColumns; ++c)
					self.Values[r][c] *= scalar;
			return self;
		}

		constexpr auto operator*(this matrix self, const matrix& other) noexcept -> matrix
		{
			matrix result{};
			for (std::uint32_t r = 0; r < VRows; ++r)
				for (std::uint32_t c = 0; c < VColumns; ++c)
					for (std::uint32_t k = 0; k < VColumns; ++k)
						result.Values[r][c] += self.Values[r][k] * other.Values[k][c];
			return result;
		}

		constexpr auto operator*=(this matrix& self, TArithmetic scalar) noexcept -> matrix&
		{
			for (std::uint32_t r = 0; r < VRows; ++r)
				for (std::uint32_t c = 0; c < VColumns; ++c)
					self.Values[r][c] *= scalar;
			return self;
		}

		// Need to consider floating point precision issues here.
		constexpr auto operator==(const matrix& other) const noexcept -> bool = default;
		constexpr auto operator!=(const matrix& other) const noexcept -> bool = default;

		TArithmetic Values[VRows][VColumns]{};
	};

	using matrix4x4_f = matrix<float, 4, 4>;

	constexpr matrix4x4_f Identity4{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	// translation is not a linear transformation, but an affine transformation.
	struct translate_matrix : matrix4x4_f
	{
		constexpr translate_matrix(const vector3_like auto& translation) noexcept
			: translate_matrix(translation.x, translation.y, translation.z)
		{ }

		constexpr translate_matrix(float tx) noexcept
			: translate_matrix(tx, tx, tx)
		{ }

		constexpr translate_matrix(float tx, float ty, float tz) noexcept
			: matrix4x4_f{
				1, 0, 0, tx,
				0, 1, 0, ty,
				0, 0, 1, tz,
				0, 0, 0, 1
			}
		{ }
	};

	// linear transformation
	struct scale_matrix : matrix4x4_f
	{
		constexpr scale_matrix(const vector_4f& scale) noexcept
			: scale_matrix(scale.x, scale.y, scale.z)
		{ }

		constexpr scale_matrix(float s) noexcept
			: scale_matrix(s, s, s)
		{ }

		constexpr scale_matrix(float sx, float sy, float sz) noexcept
			: matrix4x4_f{
				sx, 0,	0,	0,
				0,	sy, 0,	0,
				0,	0,	sz, 0,
				0,	0,	0,	1
			}
		{ }
	};

	struct x_rotation { float angle = 0; };
	struct y_rotation { float angle = 0; };
	struct z_rotation { float angle = 0; };

	// linear transformation
	struct rotation_matrix : matrix4x4_f
	{
		constexpr rotation_matrix() noexcept
			: matrix4x4_f{ Identity4 }
		{ }

		constexpr rotation_matrix(float angle) noexcept
			: rotation_matrix(angle, angle, angle)
		{ }

		constexpr rotation_matrix(const vector4_like auto& rotation) noexcept
			: rotation_matrix(rotation.x, rotation.y, rotation.z)
		{ }

		constexpr rotation_matrix(float x_angle, float y_angle, float z_angle) noexcept
			: matrix4x4_f{
				std::cos(y_angle) * std::cos(z_angle),																-std::cos(y_angle) * std::sin(z_angle),																std::sin(y_angle),						0,
				std::sin(x_angle) * std::sin(y_angle) * std::cos(z_angle) + std::cos(x_angle) * std::sin(z_angle),	-std::sin(x_angle) * std::sin(y_angle) * std::sin(z_angle) + std::cos(x_angle) * std::cos(z_angle),	-std::sin(x_angle) * std::cos(y_angle), 0,
				-std::cos(x_angle) * std::sin(y_angle) * std::cos(z_angle) + std::sin(x_angle) * std::sin(z_angle),	std::cos(x_angle) * std::sin(y_angle) * std::sin(z_angle) + std::sin(x_angle) * std::cos(z_angle),	std::cos(x_angle) * std::cos(y_angle),	0,
				0,																									0,																									0,										1
			}
		{ }

		// Note that we use a left-handed coordinate system, but this performs 
		// the same direction of rotation as a right-handed system would with 
		// the same matrix. If we wanted to be strictly correct with a 
		// left-handed coordinate system, we would flip the sign of sine terms 
		// of each of the below rotation matrices. However, the course does 
		// not do this, so I've left it as is to be consistent with the course. 
		// The relevant lecture notes discussing this can be found at 3D Matrix 
		// Transformations > 3D Direction and Handedness.
		constexpr rotation_matrix(x_rotation x_rot) noexcept
			: matrix4x4_f{
				0,  0,						0,						0,
				0,	std::cos(x_rot.angle),  -std::sin(x_rot.angle),	0,
				0,	std::sin(x_rot.angle),	std::cos(x_rot.angle),  0,
				0,	0,						0,						1
			}
		{ }

		// Flipping the sine terms switches the direction of rotation.
		// It's kept this way to be consistent with the counter-clockwise
		// rotation of the other matrices.
		constexpr rotation_matrix(y_rotation y_rot) noexcept
			: matrix4x4_f{
				std::cos(y_rot.angle),	0,	std::sin(y_rot.angle),	0,
				0,						1,	0,						0,
				-std::sin(y_rot.angle), 0,	std::cos(y_rot.angle),	0,
				0,						0,	0,						1
			}
		{ }

		constexpr rotation_matrix(z_rotation z_rot) noexcept
			: matrix4x4_f{
				std::cos(z_rot.angle),	-std::sin(z_rot.angle),	0, 0,
				std::sin(z_rot.angle),	std::cos(z_rot.angle),	0, 0,
				0,						0,						1, 0,
				0,						0,						0, 1
			}
		{ }
	};

	template<std::uint32_t VRows, std::uint32_t VColumns>
	constexpr auto operator*(float scale, const matrix<float, VRows, VColumns>& mat) 
		-> matrix<float, VRows, VColumns>
	{
		return mat * scale;
	}

	static_assert(
		[]{
			matrix4x4_f mat{ Identity4 };
			auto scaled = 4 * mat;
			mat *= 4;
			matrix4x4_f expected{
				4,	0,	0,	0,
				0,	4,	0,	0,
				0,	0,	4,	0,
				0,	0,	0,	4
			};
			if (mat != expected)
				return false;
			if (scaled != expected)
				return false;
			return true;
		}(),
		"Matrix scaling did not produce the expected results."
	);

	constexpr auto operator*(const matrix4x4_f& self, const vector_4f& other)
		noexcept -> vector_4f
	{
		return vector_4f{
			.x = self[0][0] * other.x + self[0][1] * other.y + self[0][2] * other.z + self[0][3] * other.w,
			.y = self[1][0] * other.x + self[1][1] * other.y + self[1][2] * other.z + self[1][3] * other.w,
			.z = self[2][0] * other.x + self[2][1] * other.y + self[2][2] * other.z + self[2][3] * other.w,
			.w = self[3][0] * other.x + self[3][1] * other.y + self[3][2] * other.z + self[3][3] * other.w
		};
	}

	[[deprecated("Use the projection_matrix type.")]]
	auto project(float fov_factor, math::vector_4f vec) -> math::vector_4f
	{
		/*
		* Perspective divide (perspective projection lecture):
		*   P'x = Px/Pz
		*   P'y = Py/Pz
		* where P'x and P'y are the projected points on the screen
		* and Px, Py, and Pz are the original 3-space components.
		* The idea is based on similar trianges, whose ratios
		* between sides must remain constant.
		*
		* Coordinate system handedness:
		*   Left-handed: Z-axis is positive away from the viewer.
		*   Right-handed: Z-axis is positive toward the viewer
		*
		* We scale by the fov_factor and perform the perspective
		* divide.
		*/
		return {
			.x = fov_factor * vec.x / vec.z,
			.y = fov_factor * vec.y / vec.z
		};
	}

	// Projection matrices have three main goals.
	// * Aspect ratio: adjust x and y values based on the screen width and height.
	// * Field of view: adjust x and y values based on the desired FOV.
	// * Normalization: adjust x, y, and z values to be in the range [-1,1]. This
	// is the image space that the projected coordinates will be mapped to, and is
	// also referred to as normalized device coordinates (NDC).

	struct projection_matrix : matrix4x4_f
	{
		constexpr projection_matrix(
			radians fov,
			float aspect_ratio,
			float z_near,
			float z_far
		) noexcept
			: matrix4x4_f{
				fov == 0 ? 0.f : (1.f / std::tan(fov / 2.f)) / aspect_ratio,	0,												0,							0,
				0,																(fov == 0 ? 0.f : 1.f / std::tan(fov / 2.f)),	0,							0,
				0,																0,												z_far / (z_far - z_near),	(-z_far * z_near) / (z_far - z_near),
				0,																0,												1,							0
			}
		{ }

		constexpr projection_matrix(
			degrees fov_degrees,
			float aspect_ratio,
			float z_near,
			float z_far
		) noexcept
			: projection_matrix(radians(fov_degrees), aspect_ratio, z_near, z_far)
		{ }
	};

	struct projective_perspective_divide_matrix : projection_matrix
	{
		constexpr projective_perspective_divide_matrix(
			degrees fov,
			float aspect_ratio,
			float z_near,
			float z_far
		) noexcept
			: projection_matrix{ fov, aspect_ratio, z_near, z_far }
		{ }

		constexpr projective_perspective_divide_matrix(
			radians fov,
			float aspect_ratio,
			float z_near,
			float z_far
		) noexcept
			: projection_matrix{ fov, aspect_ratio, z_near, z_far }
		{ }
	};

	// Gives us normalized device coordinates (image space) after multiplication.
	constexpr auto operator*(const projective_perspective_divide_matrix& self, const vector_4f& other) noexcept -> vector_4f
	{
		auto result = static_cast<const matrix4x4_f&>(self) * other;
		if (result.w != 0.0f)
		{
			result.x /= result.w;
			result.y /= result.w;
			result.z /= result.w;
		}
		return result;
	}
}
