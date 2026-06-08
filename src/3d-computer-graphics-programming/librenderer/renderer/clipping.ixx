export module renderer:renderer.clipping;
import std;
import :math;

export namespace renderer
{
	enum frustum_clipping_plane : unsigned
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

	struct frustum
	{
		plane planes[6]{};

		plane& left = planes[frustum_clipping_plane::left];
		plane& right = planes[frustum_clipping_plane::right];
		plane& top = planes[frustum_clipping_plane::top];
		plane& bottom = planes[frustum_clipping_plane::bottom];
		plane& near = planes[frustum_clipping_plane::near];
		plane& far = planes[frustum_clipping_plane::far];

		frustum(float fov, float z_near, float z_far)
		{
			planes[frustum_clipping_plane::left] = {
				.point = vector_3f{0.f, 0.f, 0.f},
				.normal = vector_3f{std::cos(fov / 2.f), 0.f, std::sin(fov / 2.f)}
			};

			planes[frustum_clipping_plane::right] = {
				.point = vector_3f{0.f, 0.f, 0.f},
				.normal = vector_3f{-std::cos(fov / 2.f), 0.f, std::sin(fov / 2.f)}
			};

			planes[frustum_clipping_plane::top] = {
				.point = vector_3f{0.f, 0.f, 0.f},
				.normal = vector_3f{0.f, -std::cos(fov / 2.f), std::sin(fov / 2.f)}
			};

			planes[frustum_clipping_plane::bottom] = {
				.point = vector_3f{0.f, 0.f, 0.f},
				.normal = vector_3f{0.f, std::cos(fov / 2.f), std::sin(fov / 2.f)}
			};

			planes[frustum_clipping_plane::near] = {
				.point = vector_3f{0.f, 0.f, z_near},
				.normal = vector_3f{0.f, 0.f, 1.f}
			};

			planes[frustum_clipping_plane::far] = {
				.point = vector_3f{0.f, 0.f, z_far},
				.normal = vector_3f{0.f, 0.f, -1.f}
			};
		}
	};
}
