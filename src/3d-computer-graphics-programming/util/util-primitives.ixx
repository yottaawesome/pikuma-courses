export module util:primitives;
import shared;

export
{
	struct point_2d final
	{
		uint32_t x = 0;
		uint32_t y = 0;
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
	};

	struct vector_3f
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};
}