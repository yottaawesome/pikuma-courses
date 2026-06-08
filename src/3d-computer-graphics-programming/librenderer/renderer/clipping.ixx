export module renderer:renderer.clipping;
import :math;

export namespace renderer
{
	enum class frustum_clipping_plane
	{
		left,
		right,
		top,
		bottom,
		near,
		far
	};

	struct plane
	{
		vector_3f point{};
		vector_3f normal{};
	};
}
