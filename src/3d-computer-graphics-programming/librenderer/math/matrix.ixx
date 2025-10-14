export module librenderer:matrix;
import std;
import :concepts;

export namespace math
{
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

		constexpr auto operator*=(this matrix& self, TArithmetic scalar) noexcept -> matrix&
		{
			for (std::uint32_t r = 0; r < VRows; ++r)
				for (std::uint32_t c = 0; c < VColumns; ++c)
					self.Values[r][c] *= scalar;
			return self;
		}

		TArithmetic Values[VRows][VColumns]{};
	};

	using matrix4x4_f = matrix<float, 4, 4>;

	constexpr matrix4x4_f Identity4{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	constexpr auto scale(float sx, float sy, float sz) -> matrix4x4_f
	{
		return matrix4x4_f{
			sx, 0,	0,	0,
			0,	sy, 0,	0,
			0,	0,	sz, 0,
			0,	0,	0,	1
		};
	}

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
			if (scaled[0, 0] != 4)
				return false;
			mat *= 4;
			if (mat[0, 0] != 4)
				return false;
			return true;
		}(),
		"Matrix scaling did not produce the expected results."
	);
}