export module librenderer:camera;
import std;
import :math;

export namespace renderer
{
	// The camera will have a look transformation that
	// will give us the view matrix. The view matrix is
	// (a) a translation of the world inversely from
	// the camera's position (Mt), and (b) a rotation of the
	// world inversely from the camera's orientation (Mr).
	//
	// In short, Mview = Mr * Mt, which in turn gives us the matrix
	// | r11, r12, r13, (r11tx + r12ty + r13tz), r21, r22, r23, (r21tx + r22ty + r23tz), r31, r32, r33, (r31tx + r32ty + r33tz) |
	// | r21, r22, r23, (r21tx + r22ty + r23tz), r21, r22, r23, (r21tx + r22ty + r23tz), r31, r32, r33, (r31tx + r32ty + r33tz) |
	// | r31, r32, r33, (r31tx + r32ty + r33tz), r21, r22, r23, (r21tx + r22ty + r23tz), r31, r32, r33, (r31tx + r32ty + r33tz) |
	// | 0,   0,   0,   1 |
	//
	// The translation component is a straightforward replacement
	// of the fourth column of the matrix with the negative 
	// of the camera's position, giving us:
	// | r11, r22, r23, 0| | 1, 0, 0, -eye.x|
	// | r21, r22, r23, 0| | 0, 1, 0, -eye.y|
	// | r31, r32, r33, 0| | 0, 0, 1, -eye.z|
	// | 0,   0,   0,   1| | 0, 0, 0, 1     |
	//
	// The rotation component requires the inverse of a change of 
	// basis matrix from world space to camera space, which gives us
	// | x.x,  y.x,  z.x, 0|-1 | 1, 0, 0, -eye.x|
	// | x.y,  y.y,  z.y, 0|   | 0, 1, 0, -eye.y|
	// | x.z,  y.z,  z.z, 0|   | 0, 0, 1, -eye.z|
	// | 0,    0,    0,   1|   | 0, 0, 0, 1     |
	//
	// Given the matrix is orthogonal due to it being just 
	// basis vectors, the inverse is a simple transpose, 
	// which gives us:
	// | x.x,  x.y,  x.z, 0 | | 1, 0, 0, -eye.x |
	// | y.x,  y.y,  y.z, 0 | | 0, 1, 0, -eye.y |
	// | z.x,  z.y,  z.z, 0 | | 0, 0, 1, -eye.z |
	// | 0,    0,    0,   1 | | 0, 0, 0, 1      |
	//
	// The final combined view matrix is then:
	// | x.x,  x.y,  x.z, -(x.x*eye.x + x.y*eye.y + x.z*eye.z)|
	// | y.x,  y.y,  y.z, -(y.x*eye.x + y.y*eye.y + y.z*eye.z)|
	// | z.x,  z.y,  z.z, -(z.x*eye.x + z.y*eye.y + z.z*eye.z)|
	// | 0,    0,    0,   1                                   |
	//
	// The last column is just dot products of the basis vectors 
	// with the negative camera position, thus we get:
	// | x.x,  x.y,  x.z, -dot(x, eye) |
	// | y.x,  y.y,  y.z, -dot(y, eye) |
	// | z.x,  z.y,  z.z, -dot(z, eye) |
	// | 0,    0,    0,   1            |
	struct look_at_camera
	{
		math::vector_4f eye{ .x = 0.f, .y = 0.f, .z = 0.f, .w = 0.f };
		math::vector_4f target{ .x = 0.f, .y = 0.f, .z = 1.f, .w = 0.f };
		const math::vector_4f up{ .x = 0.f, .y = 1.f, .z = 0.f, .w = 0.f };

		auto get_look_at_matrix(this const look_at_camera& self) noexcept -> math::matrix4x4_f
		{
			auto z_axis= self.target - self.eye;
			math::normalise(z_axis);
			auto x_axis = math::cross_product(self.up, z_axis);
			math::normalise(x_axis);
			auto y_axis = math::cross_product(z_axis, x_axis);
			return math::matrix4x4_f{
				x_axis.x, x_axis.y, x_axis.z, -math::dot_product(x_axis, self.eye),
				y_axis.x, y_axis.y, y_axis.z, -math::dot_product(y_axis, self.eye),
				z_axis.x, z_axis.y, z_axis.z, -math::dot_product(z_axis, self.eye),
				0.f,      0.f,      0.f,      1.f
			};
		}
	};
}